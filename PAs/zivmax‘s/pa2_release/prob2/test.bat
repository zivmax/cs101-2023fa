@echo off

:loop
echo Running testgen.exe...
.\testgen\testgen.exe

rem Add a delay if needed
timeout /nobreak /t 1 >nul

echo Running fuckingTest.exe...
.\attachments\tests\fuckingTest.exe
set "exit_code=%errorlevel%"

rem Check the exit code of the second program
echo Exit code: %exit_code%
if %exit_code% equ -1 (
    echo Second program exited with code 1. Exiting loop.
    goto :eof
)

rem Add a delay if needed
timeout /nobreak /t 1 >nul
goto :loop
