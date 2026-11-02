CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Iinclude

SRCS = src/relativistic_model.cpp \
       src/clock_noise.cpp \
       src/kalman_filter.cpp \
       src/lidar_geodesy.cpp \
       src/computer_vision.cpp \
       src/spacetime_digital_twin.cpp \
       src/data_validator.cpp \
       src/blinded_audit.cpp \
       src/daq_daemon.cpp

OBJS = $(SRCS:.cpp=.o)

all: bin/ptm_engine.exe bin/ptm_tests.exe

bin/ptm_engine.exe: $(OBJS) src/main.o
	@if not exist bin mkdir bin
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/ptm_tests.exe: $(OBJS) src/tests.o
	@if not exist bin mkdir bin
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\*.o bin\*.exe
