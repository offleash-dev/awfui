cmake_minimum_required(VERSION 3.14)

# Main demo application
add_executable(awfui_demo main.cpp)

if(AWFUI_USE_SDL)
    # SDL desktop build - link backend (which brings in awfui core + SDL)
    target_link_libraries(awfui_demo PRIVATE awfui_sdl)
else()
    # Embedded build - needs STM32 platform and ILI9341 driver
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/stm32_platform.cmake)
    target_link_libraries(awfui_demo PRIVATE awfui_adafruit adafruit_ili9341 adafruit_gfx adafruit_ft6206 stm32_platform stm32_system)
endif()

# Suppress MSVC secure CRT warnings for demo
if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    target_compile_definitions(awfui_demo PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
