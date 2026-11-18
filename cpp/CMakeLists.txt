cmake_minimum_required(VERSION 3.10)
project(PracticalTimeMachine VERSION 10.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Pure .cpp architecture (zero .hpp headers)
add_executable(ptm_engine src/main.cpp)
add_executable(ptm_tests src/tests.cpp)
