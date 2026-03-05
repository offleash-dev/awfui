#!/usr/bin/env pwsh

# =============================================================================
# Build AWFUI Keypad Example with SDL Backend
#
# This script builds the AWFUI keypad example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   .\build-keypad.ps1
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - Visual Studio 2022 or MinGW/GCC
# =============================================================================

Write-Host "Building AWFUI Keypad Example with SDL Backend..." -ForegroundColor Green

# Configure with SDL backend
Write-Host "Configuring with sdl-ninja preset..." -ForegroundColor Cyan
cmake --preset sdl-ninja -S ../.. -B ../../build/keypad-sdl -DAWFUI_BUILD_EXAMPLES=ON
if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

# Build the keypad example
Write-Host "Building awfui_keypad target..." -ForegroundColor Cyan
cmake --build ../../build/keypad-sdl --target awfui_keypad
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Run the keypad (optional)
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To run the keypad:"
Write-Host "  cd ../../build/keypad-sdl"
Write-Host "  .\examples\keypad\awfui_keypad.exe"
Write-Host ""
Write-Host "Or run directly:"
Write-Host "  .\..\..\build\keypad-sdl\examples\keypad\awfui_keypad.exe"
