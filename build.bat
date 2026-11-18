@echo off
echo ========================================================
echo Compiling TicketDesk Pro (C++ Ticket Management System)...
echo ========================================================
g++ -std=c++14 -I. src/main.cpp -o ticket_app.exe
if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Compilation successful!
    echo Run ticket_app.exe to start the application.
) else (
    echo [ERROR] Compilation failed. Please ensure g++ is installed and on your PATH.
)
pause

