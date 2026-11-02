cmake_minimum_required(VERSION 3.10)
project(PracticalTimeMachine VERSION 10.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(PTM_SOURCES
    src/relativistic_model.cpp
    src/clock_noise.cpp
    src/kalman_filter.cpp
    src/lidar_geodesy.cpp
    src/computer_vision.cpp
    src/spacetime_digital_twin.cpp
    src/data_validator.cpp
    src/blinded_audit.cpp
    src/daq_daemon.cpp
)

add_library(ptm_core STATIC ${PTM_SOURCES})

add_executable(ptm_engine src/main.cpp)
target_link_libraries(ptm_engine PRIVATE ptm_core)

add_executable(ptm_tests src/tests.cpp)
target_link_libraries(ptm_tests PRIVATE ptm_core)
