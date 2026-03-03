# AWFUI Integration Guide for Adafruit Projects

## **What to Expect**

Integrating awfui into an STM32 project with existing Adafruit libraries requires **compatibility layer setup** and **version management**. The process involves:

- **CMake configuration** for version detection
- **Compatibility headers** for older Adafruit_GFX versions
- **Build system fixes** for target conflicts
- **External dependencies** (ETL library)



## **Required Flags & Configuration**

### **Main CMakeLists.txt - Required Flags**

```cmake
# AWFUI Configuration (add before project() call)
set(AWFUI_USE_SDL OFF)        # Disable SDL for embedded builds
set(AWFUI_USE_ADAFRUIT ON)    # Enable Adafruit backend for STM32

# IMPORTANT: Point to awfui's own ArduinoCompat headers
# No need to copy any files - awfui ships with its own compatibility layer
set(ARDUINO_COMPAT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Drivers/awfui/backends/adafruit")

# Automatic version detection (required)
file(READ "${CMAKE_SOURCE_DIR}/Drivers/Adafruit/Adafruit-GFX/library.properties" ADAFRUIT_GFX_PROPERTIES)
string(REGEX MATCH "version=([0-9]+)\\.([0-9]+)\\.([0-9]+)" ADAFRUIT_GFX_VERSION_MATCH "${ADAFRUIT_GFX_PROPERTIES}")
set(ADAFRUIT_GFX_MAJOR "${CMAKE_MATCH_1}")
set(ADAFRUIT_GFX_MINOR "${CMAKE_MATCH_2}")
set(ADAFRUIT_GFX_PATCH "${CMAKE_MATCH_3}")
math(EXPR ADAFRUIT_GFX_VERSION_NUM "${ADAFRUIT_GFX_MAJOR} * 10000 + ${ADAFRUIT_GFX_MINOR} * 100 + ${ADAFRUIT_GFX_PATCH}")

# ETL dependency path
set(ETL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Drivers/ETL/include")
```

### **Drivers/CMakeLists.txt - Add Subdirectory**

```
# Add with other Adafruit libraries
add_subdirectory(./awfui)
```

### **App/CMakeLists.txt - Link Dependency**

```
target_link_libraries(${TARGET} PRIVATE
    # ... your existing dependencies ...
    awfui  # Add this
)
```



## **Possible Issues & Solutions**

### **1. ArduinoCompat Headers Missing**

**Error**: `ArduinoCompat headers were not found`

**Solution**: Set the correct path to awfui's own headers (don't copy files):

```cmake
# In main CMakeLists.txt
set(ARDUINO_COMPAT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Drivers/awfui/backends/adafruit")
```

**Important**: AWFUI ships with its own ArduinoCompat headers in `backends/adafruit/AFAdafruitCompat.h`. No file copying is required.



### **2. ETL Library Missing**

**Error**: `etl/vector.h: No such file or directory` **Solution**: Clone ETL library:

```
cd drivers
git clone https://github.com/ETLcpp/ETL.git
```



### **3. Target Name Conflicts**

**Error**: `cannot create target "adafruit_gfx" because another target with the same name already exists` **Solution**: Modify 

drivers/awfui/cmake/stm32_platform.cmake:

```
# Wrap existing add_library calls with:
if(NOT TARGET adafruit_gfx)
    # ... existing adafruit_gfx setup ...
endif()
 
if(NOT TARGET adafruit_ili9341)  
    # ... existing adafruit_ili9341 setup ...
endif()
```



### **4. SDL2 Dependencies (Optional)**

**Error**: `Could not find a package configuration file provided by "SDL2"` **Solution**: Either disable SDL (recommended for embedded):

cmake

```
set(AWFUI_USE_SDL OFF)
```

Or clone SDL2 for desktop development:

```
cd drivers
git clone https://github.com/libsdl-org/SDL.git SDL2
set(AWFUI_USE_SDL ON)
```



## **Version Compatibility Issues**

### **Older Adafruit_GFX Versions (< 1.0.x)**

Expect these missing methods:

- print(const char*) \- uses printf-style instead
- getTextBounds() \- not available
- drawRGBBitmap() \- not available
- `GFXcanvas16` - not available

**Solution**: The version-based compatibility system handles this automatically, but you may need to adjust version thresholds in 

AFDisplayAdafruitGFX.h based on your specific Adafruit_GFX version.



## **Build Verification**

**Success Indicators**:

```
[4/12] Building CXX object Drivers/awfui/CMakeFiles/awfui_adafruit.dir/backends/adafruit/AFDisplayAdafruitGFX.cpp.obj
[6/12] C:\WINDOWS\system32\cmd.exe /C "cd ... arm-none-eabi-objcopy.exe" -Oihex uw_rtos_project.elf uw_rtos_project.hex
```

**Expected Failures (OK to ignore)**:

- SDL example failures (when SDL disabled)
- awfui demo/example failures (not needed for integration)



## **Quick Integration Checklist**

- Clone awfui into `drivers/awfui`
- Clone ETL into `drivers/ETL`
- Add CMake configuration flags (including ARDUINO_COMPAT_INCLUDE_DIR)
- Fix target conflicts in stm32_platform.cmake
- Update version thresholds based on your Adafruit_GFX version
- Verify main application builds successfully

This integration works with **STM32L4** and should be adaptable to other STM32 series with minimal changes to the HAL/BSP paths.