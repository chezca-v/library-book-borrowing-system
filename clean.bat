REM filepath: d:\CODES\library-book-borrowing-system\clean.bat
@echo off
if exist library.exe del /f /q library.exe
if exist *.o del /f /q *.o
echo Clean complete.
pause