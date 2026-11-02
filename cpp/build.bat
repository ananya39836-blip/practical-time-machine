@echo off
echo ========================================================
echo Compiling Practical Time Machine (PTM) C++ Architecture
echo ========================================================

if not exist bin mkdir bin

set CXXFLAGS=-std=c++14 -O3 -Wall -Wextra -Iinclude

echo Compiling core library modules...
g++ %CXXFLAGS% -c src/relativistic_model.cpp -o src/relativistic_model.o
g++ %CXXFLAGS% -c src/clock_noise.cpp -o src/clock_noise.o
g++ %CXXFLAGS% -c src/kalman_filter.cpp -o src/kalman_filter.o
g++ %CXXFLAGS% -c src/lidar_geodesy.cpp -o src/lidar_geodesy.o
g++ %CXXFLAGS% -c src/computer_vision.cpp -o src/computer_vision.o
g++ %CXXFLAGS% -c src/spacetime_digital_twin.cpp -o src/spacetime_digital_twin.o
g++ %CXXFLAGS% -c src/data_validator.cpp -o src/data_validator.o
g++ %CXXFLAGS% -c src/blinded_audit.cpp -o src/blinded_audit.o
g++ %CXXFLAGS% -c src/daq_daemon.cpp -o src/daq_daemon.o

set CORE_OBJS=src/relativistic_model.o src/clock_noise.o src/kalman_filter.o src/lidar_geodesy.o src/computer_vision.o src/spacetime_digital_twin.o src/data_validator.o src/blinded_audit.o src/daq_daemon.o

echo Linking ptm_engine.exe...
g++ %CXXFLAGS% src/main.cpp %CORE_OBJS% -o bin/ptm_engine.exe

echo Linking ptm_tests.exe...
g++ %CXXFLAGS% src/tests.cpp %CORE_OBJS% -o bin/ptm_tests.exe

echo ========================================================
echo Compilation Successful!
echo Binaries generated: cpp/bin/ptm_engine.exe and cpp/bin/ptm_tests.exe
echo ========================================================
