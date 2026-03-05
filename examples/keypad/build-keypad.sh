#!/bin/bash

# =============================================================================
# Build AWFUI Keypad Example with SDL Backend
#
# This script builds the AWFUI keypad example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   ./build-keypad.sh
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - GCC or Clang compiler
# =============================================================================

set -e  # Exit on any error

echo "Building AWFUI Keypad Example with SDL Backend..."

# Configure with SDL backend
echo "Configuring with sdl-ninja preset..."
cmake --preset sdl-ninja -S ../.. -B ../../build/keypad-sdl -DAWFUI_BUILD_EXAMPLES=ON

# Build the keypad example
echo "Building awfui_keypad target..."
cmake --build ../../build/keypad-sdl --target awfui_keypad

echo "Build completed successfully!"
echo ""
echo "To run the keypad:"
echo "  cd ../../build/keypad-sdl"
echo "  ./examples/keypad/awfui_keypad"
echo ""
echo "Or run directly:"
echo "  ./../../build/keypad-sdl/examples/keypad/awfui_keypad"
