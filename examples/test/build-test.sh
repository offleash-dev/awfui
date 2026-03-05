#!/bin/bash

# =============================================================================
# Build AWFUI Test Example with SDL Backend
#
# This script builds the AWFUI test example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   ./build-test.sh
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - GCC or Clang compiler
# =============================================================================

set -e  # Exit on any error

echo "Building AWFUI Test Example with SDL Backend..."

# Configure with SDL backend
echo "Configuring with sdl-ninja preset..."
cmake --preset sdl-ninja -S ../.. -B ../../build/test-sdl -DAWFUI_BUILD_EXAMPLES=ON

# Build the test example
echo "Building awfui_test target..."
cmake --build ../../build/test-sdl --target awfui_test

echo "Build completed successfully!"
echo ""
echo "To run the test:"
echo "  cd ../../build/test-sdl"
echo "  ./examples/test/awfui_test"
echo ""
echo "Or run directly:"
echo "  ./../../build/test-sdl/examples/test/awfui_test"
