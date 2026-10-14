@echo off
title AlumniVerse C++ Backend Server
echo ====================================================
echo Starting AlumniVerse C++ Backend on http://localhost:8080 ...
echo ====================================================
cd /d "%~dp0backend-cpp\build"
alumniverse_backend.exe
pause
