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
option(AFUI_BUILD_EXAMPLES "Build the AFUI example applications" OFF)

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

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

# -----------------------------------------------------------------------------
# Library sources
# -----------------------------------------------------------------------------
set(AFUI_SOURCES
    src/AFButton.cpp
    src/AFDialog.cpp
    src/AFFullscreenDialog.cpp
    src/AFLabel.cpp
    src/AFModalDialog.cpp
    src/AFScreen.cpp
    src/AFScreenList.cpp
    src/AFWidget.cpp
    src/AFWorld.cpp
)

set(AFUI_HEADERS
    include/AFAdafruitCompat.h
    include/AFBase.h
    include/AFButton.h
    include/AFDialog.h
    include/AFEvent.h
    include/AFFt6206Touch.h
    include/AFFullscreenDialog.h
    include/AFLabel.h
    include/AFModalDialog.h
    include/AFResult.h
    include/AFScreen.h
    include/AFScreenList.h
    include/AFTheme.h
    include/AFTouchInterface.h
    include/AFWidget.h
    include/AFWorld.h
)

# -----------------------------------------------------------------------------
# AFUI library target
# -----------------------------------------------------------------------------
add_library(afui STATIC ${AFUI_SOURCES} ${AFUI_HEADERS})

target_include_directories(afui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

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

# Check if adafruit_gfx target exists (provided by parent project)
if(TARGET adafruit_gfx)
    target_link_libraries(afui PUBLIC adafruit_gfx)
elseif(DEFINED ADAFRUIT_GFX_INCLUDE_DIR)
    target_include_directories(afui
        PUBLIC $<BUILD_INTERFACE:${ADAFRUIT_GFX_INCLUDE_DIR}>
    )
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

# Arduino compatibility layer for non-Arduino builds (e.g., STM32)
if(DEFINED ARDUINO_COMPAT_INCLUDE_DIR)
    target_include_directories(afui
        PUBLIC $<BUILD_INTERFACE:${ARDUINO_COMPAT_INCLUDE_DIR}>
    )
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/ArduinoCompat)
    target_include_directories(afui
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Drivers/Adafruit/ArduinoCompat>
    )
endif()

# Define ARDUINO version so Adafruit headers take the right code paths
target_compile_definitions(afui PUBLIC ARDUINO=150)

# Suppress MSVC secure CRT warnings (strcpy, sprintf, etc.)
# These functions are standard C and fine for embedded targets
if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    target_compile_definitions(afui PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Embedded build settings
if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    target_compile_options(afui PRIVATE 
        -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
        -fno-exceptions -fno-rtti
    )
else()
    # Desktop builds also disable exceptions/RTTI for consistency
    target_compile_options(afui PRIVATE -fno-exceptions -fno-rtti)
endif()

# -----------------------------------------------------------------------------
# Example application (optional)
# -----------------------------------------------------------------------------
if(AFUI_BUILD_EXAMPLES)
    # Include STM32 platform configuration
    include(cmake/stm32_platform.cmake)
    
    add_executable(afui_demo examples/demo/main.cpp)
    target_link_libraries(afui_demo PRIVATE afui adafruit_ili9341 adafruit_gfx stm32_platform)
    
    # Suppress MSVC secure CRT warnings for demo
    if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
        target_compile_definitions(afui_demo PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()

# -----------------------------------------------------------------------------
# Installation (only when building standalone, not as subdirectory)
# -----------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
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

