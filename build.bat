REM filepath: d:\CODES\library-book-borrowing-system\build.bat
@echo off
setlocal enabledelayedexpansion

REM Ensure we are in the project folder
echo Building in: %CD%
echo.

REM Show timestamp of main.c to verify it was modified
if exist "main.c" (
  echo main.c last modified:
  for %%F in ("main.c") do @echo   %%~tF
) else (
  echo main.c not found in %CD%
)

echo.
REM Remove old executable to avoid running a stale binary
if exist "library.exe" (
  echo Removing old library.exe...
  del /f /q "library.exe"
)

REM Check for gcc
where gcc >nul 2>&1
if errorlevel 1 (
  echo gcc not found. Install MinGW/MSYS2/TDM-GCC or add gcc to PATH.
  echo Example: install MSYS2 (https://www.msys2.org/) or add MinGW\bin to PATH.
  pause
  exit /b 1
)

REM Collect all .c source files (handles spaces in names)
set "SRCS="
for %%F in (*.c) do (
  set "SRCS=!SRCS! "%%F""
)

if "%SRCS%"=="" (
  echo No .c source files found in %CD%.
  pause
  exit /b 1
)

echo Compiling: %SRCS%
gcc -std=c99 -Wall -Wextra %SRCS% -o "library.exe"
if errorlevel 1 (
  echo.
  echo Build failed. See errors above.
  pause
  endlocal
  exit /b 1
)

echo.
echo Build succeeded: library.exe
echo Built at %date% %time%
echo To run the program, execute: .\library.exe
pause
endlocal