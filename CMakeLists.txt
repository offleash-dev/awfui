cmake_minimum_required(VERSION 3.14)
project(AFUI
    VERSION 0.1.0
    DESCRIPTION "Adafruit UI Framework for touch LCD displays"
    LANGUAGES CXX
)

#set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


 
# -----------------------------------------------------------------------------
# Options
# -----------------------------------------------------------------------------
option(AFUI_BUILD_EXAMPLES "Build the AFUI example applications" ON)
option(AFUI_USE_SDL "Build with SDL backend for desktop simulation" ON)
option(AFUI_USE_ADAFRUIT "Build with Adafruit_GFX-based backend" ON)
 
# -----------------------------------------------------------------------------
# Compiler settings (GCC-friendly, cross-platform)
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -Wall 
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
endif()

if(MSVC)
    add_compile_options(/W4)
endif()

# -----------------------------------------------------------------------------
# Library sources
# -----------------------------------------------------------------------------
set(AFUI_SOURCES
    src/AFDisplayInterface.cpp
    src/AFButton.cpp
    src/AFCheckbox.cpp
    src/AFPanel.cpp
    src/AFFullscreenDialog.cpp
    src/AFImage.cpp
    src/AFImageWidget.cpp
    src/AFImageButton.cpp
    src/AFLabel.cpp
    src/AFLayoutHelpers.cpp
    src/AFModalDialog.cpp
    src/AFProgressBar.cpp
    src/AFRadioButton.cpp
    src/AFRadioButtonGroup.cpp
    src/AFScreen.cpp
    src/AFScreenList.cpp
    src/AFSlider.cpp
    src/AFWidget.cpp
    src/AFWorld.cpp
)

set(AFUI_HEADERS
    include/AFBase.h
    include/AFButton.h
    include/AFCheckbox.h
    include/AFPanel.h
    include/AFDisplayInterface.h
    include/AFEvent.h
    include/AFEventQueue.h
    include/AFFullscreenDialog.h
    include/AFImage.h
    include/AFImageWidget.h
    include/AFImageButton.h
    include/AFLabel.h
    include/AFLayoutHelpers.h
    include/AFModalDialog.h
    include/AFProgressBar.h
    include/AFRadioButton.h
    include/AFRadioButtonGroup.h
    include/AFResult.h
    include/AFScreen.h
    include/AFScreenList.h
    include/AFSlider.h
    include/AFValueWidget.h
    include/AFTheme.h
    include/AFTouchInterface.h
    include/AFWidget.h
    include/AFWorld.h
)

# -----------------------------------------------------------------------------
# AFUI core library target (framework only, no backend)
# -----------------------------------------------------------------------------
add_library(afui STATIC ${AFUI_SOURCES} ${AFUI_HEADERS})

target_include_directories(afui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# -----------------------------------------------------------------------------
# Adafruit backend target (display + touch for Adafruit_GFX-based drivers)
# -----------------------------------------------------------------------------
if(AFUI_USE_ADAFRUIT)
    add_library(afui_adafruit STATIC
        backends/adafruit/AFDisplayAdafruitGFX.cpp
        backends/adafruit/AFAdafruitCompat.h
        backends/adafruit/AFDisplayAdafruitGFX.h
        backends/adafruit/AFFt6206Touch.h
    )
    target_include_directories(afui_adafruit
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends/adafruit>
    )
    target_link_libraries(afui_adafruit PUBLIC afui)
endif()

# -----------------------------------------------------------------------------
# SDL backend target (optional)
# -----------------------------------------------------------------------------
if(AFUI_USE_SDL)
    add_library(afui_sdl STATIC
        backends/sdl/AFDisplaySDL.cpp
        backends/sdl/AFTouchSDL.cpp
        backends/sdl/AFDisplaySDL.h
        backends/sdl/AFTouchSDL.h
    )
    target_include_directories(afui_sdl
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends/sdl>
    )
    target_link_libraries(afui_sdl PUBLIC afui)
endif()

# -----------------------------------------------------------------------------
# Dependencies
#
# This library depends on:
#   - Adafruit_GFX (graphics primitives)
#   - ETL (Embedded Template Library)
#
# When used as a subdirectory in a larger project, link against targets.
# When building standalone, provide paths via cache variables.
# -----------------------------------------------------------------------------

if(AFUI_USE_ADAFRUIT)
    # Check if adafruit_gfx target exists (provided by parent project)
    if(TARGET adafruit_gfx)
        target_link_libraries(afui_adafruit PUBLIC adafruit_gfx)
    elseif(DEFINED ADAFRUIT_GFX_INCLUDE_DIR)
        target_include_directories(afui_adafruit
            PUBLIC $<BUILD_INTERFACE:${ADAFRUIT_GFX_INCLUDE_DIR}>
        )
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/Adafruit-GFX)
        # Default: assume Adafruit_GFX is in Drivers/Adafruit/Adafruit-GFX
        target_include_directories(afui_adafruit
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/Adafruit-GFX>
        )
    else()
        message(FATAL_ERROR "AFUI_USE_ADAFRUIT is ON but Adafruit_GFX headers were not found. Set ADAFRUIT_GFX_INCLUDE_DIR or disable AFUI_USE_ADAFRUIT.")
    endif()
endif()

# ETL (header-only library)
if(DEFINED ETL_INCLUDE_DIR)
    target_include_directories(afui
        PUBLIC $<BUILD_INTERFACE:${ETL_INCLUDE_DIR}>
    )
else()
    # Default: assume ETL is a sibling in modules directory
    target_include_directories(afui
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../etl/include>
    )
endif()

if(AFUI_USE_ADAFRUIT)
    # Arduino compatibility layer for non-Arduino builds (e.g., STM32)
    if(DEFINED ARDUINO_COMPAT_INCLUDE_DIR)
        target_include_directories(afui_adafruit
            PUBLIC $<BUILD_INTERFACE:${ARDUINO_COMPAT_INCLUDE_DIR}>
        )
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/ArduinoCompat)
        target_include_directories(afui_adafruit
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/ArduinoCompat>
        )
    else()
        message(FATAL_ERROR "AFUI_USE_ADAFRUIT is ON but ArduinoCompat headers were not found. Set ARDUINO_COMPAT_INCLUDE_DIR or disable AFUI_USE_ADAFRUIT.")
    endif()
endif()

# Define ARDUINO version so Adafruit headers take the right code paths
target_compile_definitions(afui PUBLIC ARDUINO=150)

# -----------------------------------------------------------------------------
# SDL dependency (when building with SDL backend)
# -----------------------------------------------------------------------------
if(AFUI_USE_SDL)
    # Check for SDL2 source as sibling directory
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2/CMakeLists.txt)
        message(STATUS "Adding SDL2 from ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2")
        # Build SDL2 as static library for cross-platform simplicity
        set(SDL_SHARED OFF CACHE BOOL "Build SDL2 shared library" FORCE)
        set(SDL_STATIC ON CACHE BOOL "Build SDL2 static library" FORCE)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SDL2 ${CMAKE_BINARY_DIR}/SDL2)
        # Link static SDL2 - we use SDL_MAIN_HANDLED so no SDL2main needed
        if(TARGET SDL2::SDL2-static)
            target_link_libraries(afui_sdl PUBLIC SDL2::SDL2-static)
        elseif(TARGET SDL2::SDL2)
            target_link_libraries(afui_sdl PUBLIC SDL2::SDL2)
        elseif(TARGET SDL3::SDL3-static)
            target_link_libraries(afui_sdl PUBLIC SDL3::SDL3-static)
        elseif(TARGET SDL3::SDL3)
            target_link_libraries(afui_sdl PUBLIC SDL3::SDL3)
        else()
            message(FATAL_ERROR "SDL was added from ../SDL2 but no known SDL2/SDL3 CMake targets were found.")
        endif()
    else()
        # Fall back to system-installed SDL2
        find_package(SDL2 REQUIRED)
        target_link_libraries(afui_sdl PUBLIC SDL2::SDL2)
    endif()
    target_compile_definitions(afui_sdl PUBLIC AFUI_USE_SDL=1)
endif()

# Suppress MSVC secure CRT warnings (strcpy, sprintf, etc.)
# These functions are standard C and fine for embedded targets
set(AFUI_ALL_TARGETS afui)
if(AFUI_USE_SDL)
    list(APPEND AFUI_ALL_TARGETS afui_sdl)
endif()
if(AFUI_USE_ADAFRUIT)
    list(APPEND AFUI_ALL_TARGETS afui_adafruit)
endif()

if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    foreach(tgt ${AFUI_ALL_TARGETS})
        target_compile_definitions(${tgt} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endforeach()
endif()

# MSVC doesn't have GCC's __builtin_bswap* - force-include compatibility header
if(MSVC)
    foreach(tgt ${AFUI_ALL_TARGETS})
        target_compile_options(${tgt} PRIVATE 
            /FI"${CMAKE_CURRENT_SOURCE_DIR}/include/AFCompilerCompat.h"
        )
    endforeach()
endif()

# Embedded build settings
if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    foreach(tgt ${AFUI_ALL_TARGETS})
        target_compile_options(${tgt} PRIVATE 
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
            -fno-exceptions -fno-rtti
        )
    endforeach()
elseif(MSVC)
    # MSVC desktop builds - disable exceptions/RTTI for consistency with embedded
    foreach(tgt ${AFUI_ALL_TARGETS})
        target_compile_options(${tgt} PRIVATE /EHs-c- /GR-)
    endforeach()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang desktop builds
    foreach(tgt ${AFUI_ALL_TARGETS})
        target_compile_options(${tgt} PRIVATE -fno-exceptions -fno-rtti)
    endforeach()
endif()

# -----------------------------------------------------------------------------
# Example applications (optional)
# -----------------------------------------------------------------------------
if(AFUI_BUILD_EXAMPLES)
    # Main demo
    add_executable(afui_demo examples/demo/main.cpp)
    
    # Coordinate/rotation test demo
    add_executable(afui_test examples/test/main.cpp)

    # Pure-SDL demo (no Adafruit_GFX dependency from app code)
    add_executable(afui_justsdl examples/demo-justSdl/main.cpp)

    # Keypad demo
    add_executable(afui_keypad examples/keypad/main.cpp)
    
    if(AFUI_USE_SDL)
        # SDL desktop build - link backend (which brings in afui core + SDL)
        target_link_libraries(afui_demo PRIVATE afui_sdl)
        target_link_libraries(afui_test PRIVATE afui_sdl)
        target_link_libraries(afui_justsdl PRIVATE afui_sdl)
        target_link_libraries(afui_keypad PRIVATE afui_sdl)
    else()
        # Embedded build - needs STM32 platform and ILI9341 driver
        include(cmake/stm32_platform.cmake)
        target_link_libraries(afui_demo PRIVATE afui_adafruit adafruit_ili9341 adafruit_gfx stm32_platform)
        target_link_libraries(afui_test PRIVATE afui_adafruit adafruit_ili9341 adafruit_gfx stm32_platform)
    endif()
    
    # Suppress MSVC secure CRT warnings for demos
    if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
        target_compile_definitions(afui_demo PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_definitions(afui_test PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_definitions(afui_justsdl PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_compile_definitions(afui_keypad PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()

# -----------------------------------------------------------------------------
# Installation (only when building standalone, not as subdirectory, and not SDL)
# -----------------------------------------------------------------------------
# Skip install rules when using SDL subdirectory - the SDL targets aren't exportable
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR AND NOT AFUI_USE_SDL)
    include(GNUInstallDirs)

    install(TARGETS afui
        EXPORT afui-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(FILES ${AFUI_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/afui
    )

    install(EXPORT afui-targets
        FILE afui-targets.cmake
        NAMESPACE afui::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/afui
    )
endif()

