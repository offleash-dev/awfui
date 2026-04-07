@echo off
echo Copying AWFUI directories to lib/awfui...

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0
set ELECROW_DIR=%~dp0
set LIB_DIR=%ELECROW_DIR%\lib\awfui

REM Check if main AWFUI repo exists
if not exist "%SCRIPT_DIR%..\..\include" (
    echo Error: Main AWFUI repo not found at %SCRIPT_DIR%..\..\
    pause
    exit /b 1
)

REM Create lib directory if it doesn't exist
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"

REM Remove existing directories
if exist "%LIB_DIR%\include" rmdir /s /q "%LIB_DIR%\include"
if exist "%LIB_DIR%\src" rmdir /s /q "%LIB_DIR%\src"
if exist "%LIB_DIR%\backends" rmdir /s /q "%LIB_DIR%\backends"

REM Copy directories
echo Copying include...
xcopy "%SCRIPT_DIR%..\..\include" "%LIB_DIR%\include" /E /I /Y

echo Copying src...
xcopy "%SCRIPT_DIR%..\..\src" "%LIB_DIR%\src" /E /I /Y

echo Copying backends...
xcopy "%SCRIPT_DIR%..\..\backends" "%LIB_DIR%\backends" /E /I /Y

REM Copy backend implementation to working project's src directory
echo Copying AFDisplayLovyanGFX.cpp to working project src...
if not exist "%ELECROW_DIR%\src" mkdir "%ELECROW_DIR%\src"
copy "%LIB_DIR%\backends\Lovyan\AFDisplayLovyanGFX.cpp" "%ELECROW_DIR%\src\AFDisplayLovyanGFX.cpp"

echo Copy complete!
echo.
echo AWFUI has been copied to: %LIB_DIR%
echo Backend implementation copied to: %ELECROW_DIR%\src\
echo.
pause
