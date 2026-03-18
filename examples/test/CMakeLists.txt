cmake_minimum_required(VERSION 3.14)

# Coordinate/rotation test demo
add_executable(awfui_test testUi.cpp main.cpp sliderKeyboardExample.cpp afsliderkeyboard.cpp)

if(AWFUI_USE_SDL)
    # SDL desktop build - link backend (which brings in awfui core + SDL)
    target_link_libraries(awfui_test PRIVATE awfui_sdl)
else()
    # Embedded build - needs STM32 platform and ILI9341 driver
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/stm32_platform.cmake)
    target_link_libraries(awfui_test PRIVATE awfui_adafruit adafruit_ili9341 adafruit_gfx adafruit_ft6206 stm32_platform stm32_system)
endif()

# Suppress MSVC secure CRT warnings for test
if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    target_compile_definitions(awfui_test PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
