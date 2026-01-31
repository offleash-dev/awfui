@echo off
setlocal

echo === Building AFUI Demo ===

REM Get the directory where this script is located
set "SCRIPT_DIR=%~dp0"

REM Create build directory
if not exist build (
    mkdir build
)

cd build

REM Configure CMake with absolute paths to dependencies
REM Note: STM32 platform paths are configured in cmake/stm32_platform.cmake
cmake .. ^
  -G "Ninja" ^
  -DCMAKE_CXX_COMPILER=clang++ ^
  -DAFUI_BUILD_EXAMPLES=ON ^
  -DETL_INCLUDE_DIR="%SCRIPT_DIR%..\etl\include" ^
  -DADAFRUIT_GFX_INCLUDE_DIR="%SCRIPT_DIR%..\Drivers\Adafruit\Adafruit-GFX" ^
  -DARDUINO_COMPAT_INCLUDE_DIR="%SCRIPT_DIR%..\Drivers\Adafruit\ArduinoCompat"


if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build
cmake --build . --config Release

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo.
echo Demo built successfully.
echo Executable is in: build\examples\demo\Release\
echo.

endlocal