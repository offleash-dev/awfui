#!/usr/bin/env pwsh

# =============================================================================
# Build AWFUI Test Example with SDL Backend
#
# This script builds the AWFUI test example using the SDL backend
# for desktop testing and development.
#
# Usage:
#   .\build-test.ps1
#
# Requirements:
#   - CMake 3.14+
#   - SDL2 development libraries
#   - Visual Studio 2022 or MinGW/GCC
# =============================================================================

Write-Host "Building AWFUI Test Example with SDL Backend..." -ForegroundColor Green

# Configure with SDL backend
Write-Host "Configuring with sdl-ninja preset..." -ForegroundColor Cyan
cmake --preset sdl-ninja -S ../.. -B ../../build/test-sdl -DAWFUI_BUILD_EXAMPLES=ON
if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

# Build the test example
Write-Host "Building awfui_test target..." -ForegroundColor Cyan
cmake --build ../../build/test-sdl --target awfui_test
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# Run the test (optional)
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To run the test:"
Write-Host "  cd ../../build/test-sdl"
Write-Host "  .\examples\test\awfui_test.exe"
Write-Host ""
Write-Host "Or run directly:"
Write-Host "  .\..\..\build\test-sdl\examples\test\awfui_test.exe"
