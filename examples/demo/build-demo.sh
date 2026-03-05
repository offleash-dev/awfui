#!/bin/bash

# =============================================================================
# Build AWFUI Demo Example with SDL Backend
#
# This script builds the AWFUI demo example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   ./build-demo.sh
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - GCC or Clang compiler
# =============================================================================

set -e  # Exit on any error

echo "Building AWFUI Demo Example with SDL Backend..."

# Configure with SDL backend
echo "Configuring with sdl-ninja preset..."
cmake --preset sdl-ninja -S ../.. -B ../../build/demo-sdl -DAWFUI_BUILD_EXAMPLES=ON

# Build the demo example
echo "Building awfui_demo target..."
cmake --build ../../build/demo-sdl --target awfui_demo

echo "Build completed successfully!"
echo ""
echo "To run the demo:"
echo "  cd ../../build/demo-sdl"
echo "  ./examples/demo/awfui_demo"
echo ""
echo "Or run directly:"
echo "  ./../../build/demo-sdl/examples/demo/awfui_demo"
