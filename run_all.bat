@echo off
title AlumniVerse - Start All Services
echo ====================================================
echo Starting AlumniVerse Backend and Frontend ...
echo ====================================================

start "AlumniVerse C++ Backend (Port 8080)" cmd /k "cd /d %~dp0backend-cpp\build && alumniverse_backend.exe"

timeout /t 2 /nobreak >nul

start "AlumniVerse React Frontend (Port 5173)" cmd /k "cd /d %~dp0frontend && npm run dev"

echo All services launched!
echo - Frontend: http://localhost:5173/explorer
echo - Backend:  http://localhost:8080/api/v1/health
echo.
pause
