@echo off
echo Copying changes from lib/awfui back to main AWFUI repo...

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0
set ELECROW_DIR=%dp0
set LIB_DIR=%ELECROW_DIR%\lib\awfui

REM Check if lib directory exists
if not exist "%LIB_DIR%" (
    echo Error: lib/awfui directory not found!
    echo Please run copy_to_lib.bat first.
    pause
    exit /b 1
)

REM Check if main AWFUI repo exists
if not exist "%SCRIPT_DIR%..\..\include" (
    echo Error: Main AWFUI repo not found at %SCRIPT_DIR%..\..\
    pause
    exit /b 1
)

REM Copy directories back
echo Copying include back...
xcopy "%LIB_DIR%\include" "%SCRIPT_DIR%..\..\include" /E /I /Y

echo Copying src back...
xcopy "%LIB_DIR%\src" "%SCRIPT_DIR%..\..\src" /E /I /Y

echo Copying backends back...
xcopy "%LIB_DIR%\backends" "%SCRIPT_DIR%..\..\backends" /E /I /Y

REM Copy backend implementation from working project's src directory back
echo Copying AFDisplayLovyanGFX.cpp from working project src back to backend...
if exist "%ELECROW_DIR%\src\AFDisplayLovyanGFX.cpp" (
    copy "%ELECROW_DIR%\src\AFDisplayLovyanGFX.cpp" "%LIB_DIR%\backends\Lovyan\AFDisplayLovyanGFX.cpp"
    echo Backend implementation copied back to: %LIB_DIR%\backends\Lovyan\
) else (
    echo Warning: AFDisplayLovyanGFX.cpp not found in working project src
)

echo Copy complete!
echo.
echo Changes have been copied back to main AWFUI repo: %SCRIPT_DIR%..\..
echo.
pause
