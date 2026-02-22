@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PRESET=%~1"
if "%PRESET%"=="" set "PRESET=sdl-ninja"

echo === Building AFUI (%PRESET%) ===

cmake --preset %PRESET% -S "%SCRIPT_DIR%"
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

cmake --build --preset %PRESET%
if errorlevel 1 (
    REM Fallback: build directly from the build directory
    cmake --build "%SCRIPT_DIR%build\%PRESET%"
    if errorlevel 1 (
        echo Build failed.
        exit /b 1
    )
)

echo.
echo Build successful (%PRESET%).
echo.

endlocal