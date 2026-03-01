@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "PRESET="
set "CLEAN="
set "TARGET="

:parse_args
if "%~1"=="" goto done_args
if "%~1"=="-a" (
    set "CLEAN=--clean-first"
    shift
    goto parse_args
)
if "%~1"=="-aw" (
    set "CLEAN=--clean-first"
    set "TARGET=--target awfui"
    shift
    goto parse_args
)
if "%~1"=="-as" (
    set "CLEAN=--clean-first"
    set "TARGET=--target awfui_sdl"
    shift
    goto parse_args
)
if "%~1"=="-ae" (
    set "CLEAN=--clean-first"
    set "TARGET=--target awfui_demo --target awfui_test --target awfui_justsdl --target awfui_keypad"
    shift
    goto parse_args
)
if not defined PRESET (
    set "PRESET=%~1"
    shift
    goto parse_args
)
shift
goto parse_args
:done_args

if not defined PRESET set "PRESET=sdl-ninja"

if defined CLEAN (
    echo === Rebuilding AWFUI [%PRESET%] ===
) else (
    echo === Building AWFUI [%PRESET%] ===
)

cmake --preset %PRESET% -S "%SCRIPT_DIR%"
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

cmake --build --preset %PRESET% %CLEAN% %TARGET%
if errorlevel 1 (
    REM Fallback: build directly from the build directory
    cmake --build "%SCRIPT_DIR%build\%PRESET%" %CLEAN% %TARGET%
    if errorlevel 1 (
        echo Build failed.
        exit /b 1
    )
)

echo.
echo Build successful (%PRESET%).
echo.

endlocal