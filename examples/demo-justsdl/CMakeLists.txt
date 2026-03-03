cmake_minimum_required(VERSION 3.14)

# Pure-SDL demo (no Adafruit_GFX dependency from app code)
add_executable(awfui_justsdl main.cpp)

# This demo only works with SDL backend
if(AWFUI_USE_SDL)
    target_link_libraries(awfui_justsdl PRIVATE awfui_sdl)
else()
    message(WARNING "awfui_justsdl demo requires SDL backend (AWFUI_USE_SDL=ON)")
    return()
endif()

# Suppress MSVC secure CRT warnings for demo
if(MSVC OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    target_compile_definitions(awfui_justsdl PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
