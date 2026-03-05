# =============================================================================
# STM32 Platform Configuration for AWFUI
# 
# This file sets up all STM32-related include directories and definitions
# needed to build the demo application on STM32L475E-IOT01 board.
#
# Usage in CMakeLists.txt:
#   include(cmake/stm32_platform.cmake)
#   target_link_libraries(my_target PRIVATE stm32_platform)
# =============================================================================

# Determine the modules directory (parent of awfui)
get_filename_component(AWFUI_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
get_filename_component(AWFUI_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

# Create an interface library to hold all STM32 platform settings
if(NOT TARGET stm32_platform)
add_library(stm32_platform INTERFACE)
endif()

# -----------------------------------------------------------------------------
# STM32 Include Directories
# -----------------------------------------------------------------------------
target_include_directories(stm32_platform INTERFACE
    # Adafruit drivers
    ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit_ILI9341
    
    # STM32 BSP (Board Support Package)
    ${AWFUI_MODULES_DIR}/Drivers/BSP/B-L475E-IOT01
    
    # STM32 HAL (Hardware Abstraction Layer)
    ${AWFUI_MODULES_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc
    
    # Board configuration (project-specific)
    ${AWFUI_ROOT_DIR}/Configuration
    
    # CMSIS Device headers (STM32L4xx specific)
    ${AWFUI_MODULES_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    
    # CMSIS Core headers (ARM Cortex-M)
    ${AWFUI_MODULES_DIR}/Drivers/CMSIS/Include
)

# -----------------------------------------------------------------------------
# STM32 Compile Definitions
# -----------------------------------------------------------------------------
target_compile_definitions(stm32_platform INTERFACE
    # Target MCU
    STM32L475xx
    
    # Use HAL driver
    USE_HAL_DRIVER
)

# Clang on Windows may not define __GNUC__ which CMSIS needs to select the right compiler header.
# Define it so CMSIS uses cmsis_gcc.h (Clang is GCC-compatible for intrinsics)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32)
    target_compile_definitions(stm32_platform INTERFACE __GNUC__=4)
    
    # Suppress warnings from STM32 HAL headers that assume 32-bit pointers
    # (pointer-to-int and int-to-pointer casts are safe on actual STM32 target)
    target_compile_options(stm32_platform INTERFACE
        -Wno-pointer-to-int-cast
        -Wno-int-to-pointer-cast
    )
endif()

# -----------------------------------------------------------------------------
# Adafruit Libraries (source files needed for linking)
# -----------------------------------------------------------------------------
if(NOT TARGET adafruit_gfx)
    add_library(adafruit_gfx STATIC
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit-GFX/Adafruit_GFX.cpp
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit-GFX/glcdfont.c
    )
    target_include_directories(adafruit_gfx PUBLIC
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit-GFX
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/ArduinoCompat
    )
    target_compile_definitions(adafruit_gfx PUBLIC ARDUINO=150)
    target_link_libraries(adafruit_gfx PUBLIC stm32_platform)

    # Suppress warnings in Adafruit code we don't control
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(adafruit_gfx PRIVATE -Wno-unused-parameter)
    endif()

    # Suppress MSVC secure CRT warnings in Adafruit code
    if(WIN32)
        target_compile_definitions(adafruit_gfx PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()

if(NOT TARGET adafruit_ili9341)
    add_library(adafruit_ili9341 STATIC
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit_ILI9341/Adafruit_ILI9341.cpp
    )
    target_include_directories(adafruit_ili9341 PUBLIC
        ${AWFUI_MODULES_DIR}/Drivers/Adafruit/Adafruit_ILI9341
    )
    target_link_libraries(adafruit_ili9341 PUBLIC adafruit_gfx stm32_platform)

    # Suppress MSVC secure CRT warnings in Adafruit code
    if(WIN32)
        target_compile_definitions(adafruit_ili9341 PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()

# HAL/BSP stubs for Windows build testing (real implementations are on STM32)
if(WIN32 AND NOT TARGET hal_stubs)
    add_library(hal_stubs STATIC
        ${CMAKE_CURRENT_LIST_DIR}/hal_stubs.cpp
    )
    target_link_libraries(adafruit_ili9341 PUBLIC hal_stubs)
endif()

# -----------------------------------------------------------------------------
# Optional: Print configuration for debugging
# -----------------------------------------------------------------------------
message(STATUS "STM32 Platform configured:")
message(STATUS "  Modules dir: ${AWFUI_MODULES_DIR}")
message(STATUS "  Root dir: ${AWFUI_ROOT_DIR}")
