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
    awfui          # Core library
    awfui_adafruit # Adafruit backend
    # Optional: Use --whole-archive if needed for vtable issues
    # -Wl,--whole-archive awfui_adafruit awfui -Wl,--no-whole-archive
)
```

### **CMake Options for Integration**

Control AWFUI build behavior with these options (set before `add_subdirectory`):

```cmake
# Disable examples when integrating (recommended)
set(AWFUI_BUILD_EXAMPLES OFF)

# Backend selection
set(AWFUI_USE_SDL OFF)        # Disable SDL for embedded builds
set(AWFUI_USE_ADAFRUIT ON)    # Enable Adafruit backend for STM32
```

**Usage Examples:**
```bash
# Command line:
cmake -DAWFUI_BUILD_EXAMPLES=OFF -DAWFUI_USE_SDL=OFF path/to/project

# Or in CMakeLists.txt:
set(AWFUI_BUILD_EXAMPLES OFF)
set(AWFUI_USE_SDL OFF)
add_subdirectory(./awfui)
```



## **Modular Structure (New)**

AWFUI now has a modular build structure that separates the core library from examples:

```
drivers/awfui/
├── CMakeLists.txt          # Main library configuration
├── examples/               # Example applications (optional)
│   ├── CMakeLists.txt      # Examples build configuration  
│   ├── demo/               # Main demo
│   ├── test/               # Test/demo
│   ├── demo-justsdl/       # SDL-only demo
│   └── keypad/             # Keypad demo (great starting point)
├── include/                # Public headers
├── src/                    # Library source files
├── backends/               # Backend implementations
│   ├── adafruit/           # Adafruit_GFX backend
│   └── sdl/                # SDL backend
└── cmake/                  # Platform support files
```

**Benefits:**
- **Library-only builds** - Build just the AWFUI library without examples
- **Selective examples** - Build only specific examples you need
- **Better organization** - Clear separation of library code and demos
- **Easier maintenance** - Examples don't interfere with library builds

## **RTOS Task Integration**

AWFUI works well with RTOS environments. Here's a typical integration pattern:

```cpp
// tasks/task_gui.cpp
#include "AFScreen.h"
#include "AFButton.h"
#include "AFWorld.h"

#if USE_SDL
#include "AFDisplaySDL.h"
#include "AFTouchSDL.h"
#else
#include "AFDisplayAdafruitGFX.h"
#include "AFFt6206Touch.h"
#endif

// Global objects (initialized in task)
AFDisplayAdafruitGFX* display = nullptr;
AFFt6206Touch*       touch = nullptr;
AFWorld*             world = nullptr;

void GuiTask(void* pdata) {
    // Initialize display and touch
    Adafruit_ILI9341 tft;
    touch = new AFFt6206Touch();
    display = new AFDisplayAdafruitGFX(tft);
    
    // Setup AWFUI
    display->begin();
    touch->begin();
    AFWorld::init(*display, touch, &eventQueue);
    world = AFWorld::instance();
    
    // Create your UI screens here
    setupMyScreens();
    
    // Main GUI loop
    while (true) {
        world->loop();
        display->present();
        OSTimeDly(16); // ~60 FPS
    }
}
```

**Key Points:**
- Use pointers for global objects to avoid initialization order issues
- Initialize AWFUI after your hardware is ready
- Use RTOS delays for frame rate control
- Protect SPI/shared resources with locks if needed

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



### **5. Vtable/Typeinfo Linking Issues**

**Error**: `undefined reference to 'typeinfo for AFScreen'`

**Solution**: Use `--whole-archive` to force inclusion of all vtable symbols:

```cmake
target_link_libraries(${TARGET} PRIVATE
    # ... other dependencies ...
    -Wl,--whole-archive
    awfui_adafruit
    awfui
    -Wl,--no-whole-archive
)
```

**Alternative**: Add a dummy reference to force vtable emission:

```cpp
// In one of your source files
extern "C" void force_awfui_vtables() {
    AFScreen* dummy = nullptr;
    dummy->~AFScreen(); // Forces vtable inclusion
}
```

### **6. RTTI and Embedded Optimization (Important)**

**Issue**: `undefined reference to 'typeinfo for AFScreen'` even when not using RTTI features

**Root Cause**: RTTI is enabled by default in C++, but embedded systems should disable it to save space and avoid typeinfo metadata.

**Solution**: Always disable RTTI for embedded builds:

```cmake
# In App/CMakeLists.txt - C++ only
target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
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