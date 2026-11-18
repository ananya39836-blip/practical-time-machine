@echo off
setlocal enabledelayedexpansion
echo ========================================================
echo Compiling Practical Time Machine (PTM) Pure C++ Architecture
echo ========================================================

if exist "C:\MinGW\bin\g++.exe" set "PATH=C:\MinGW\bin;%PATH%"
if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\ucrt64\bin\g++.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] g++ compiler not found. Please install MinGW-w64 / MSYS2 and add bin to PATH.
    exit /b 1
)

if not exist bin mkdir bin

set CXXFLAGS=-std=c++14 -O3 -Wall -Wextra -Isrc

echo Building ptm_engine.exe from pure .cpp modules...
g++ %CXXFLAGS% src/main.cpp -o bin/ptm_engine.exe || exit /b 1

echo Building ptm_tests.exe from pure .cpp modules...
g++ %CXXFLAGS% src/tests.cpp -o bin/ptm_tests.exe || exit /b 1

echo ========================================================
echo Compilation Successful!
echo Binaries generated: cpp/bin/ptm_engine.exe and cpp/bin/ptm_tests.exe
echo ========================================================
