cmake_minimum_required(VERSION 3.14)

# -----------------------------------------------------------------------------
# Options for building examples
# -----------------------------------------------------------------------------
option(AWFUI_BUILD_DEMO "Build the main AWFUI demo application" ON)
option(AWFUI_BUILD_TEST "Build the AWFUI test application" ON)
option(AWFUI_BUILD_JUSTSDL "Build the pure-SDL demo application" ON)
option(AWFUI_BUILD_KEYPAD "Build the keypad demo application" ON)

# -----------------------------------------------------------------------------
# Example applications (optional)
# -----------------------------------------------------------------------------
if(AWFUI_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if(AWFUI_BUILD_TEST)
    add_subdirectory(test)
endif()

if(AWFUI_BUILD_JUSTSDL)
    add_subdirectory(demo-justsdl)
endif()

if(AWFUI_BUILD_KEYPAD)
    add_subdirectory(keypad)
endif()
