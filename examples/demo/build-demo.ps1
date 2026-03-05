#!/usr/bin/env pwsh

# =============================================================================
# Build AWFUI Demo Example with SDL Backend
#
# This script builds the AWFUI demo example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   .\build-demo.ps1
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - Visual Studio 2022 or MinGW/GCC
# =============================================================================

Write-Host "Building AWFUI Demo Example with SDL Backend..." -ForegroundColor Green

# Configure with SDL backend
Write-Host "Configuring with sdl-ninja preset..." -ForegroundColor Cyan
cmake --preset sdl-ninja -S ../.. -B ../../build/demo-sdl -DAWFUI_BUILD_EXAMPLES=ON
if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

# Build the demo example
Write-Host "Building awfui_demo target..." -ForegroundColor Cyan
cmake --build ../../build/demo-sdl --target awfui_demo
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Run the demo (optional)
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To run the demo:"
Write-Host "  cd ../../build/demo-sdl"
Write-Host "  .\examples\demo\awfui_demo.exe"
Write-Host ""
Write-Host "Or run directly:"
Write-Host "  .\..\..\build\demo-sdl\examples\demo\awfui_demo.exe"
