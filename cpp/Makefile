CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Isrc

ifeq ($(OS),Windows_NT)
	EXT = .exe
	MKDIR = if not exist bin mkdir bin
	RM = del /Q bin\*.exe
else
	EXT =
	MKDIR = mkdir -p bin
	RM = rm -f bin/ptm_engine bin/ptm_tests
endif

all: bin/ptm_engine$(EXT) bin/ptm_tests$(EXT)

bin/ptm_engine$(EXT): src/main.cpp src/*.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $@

bin/ptm_tests$(EXT): src/tests.cpp src/*.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) src/tests.cpp -o $@

test: bin/ptm_tests$(EXT)
	./bin/ptm_tests$(EXT)

run: bin/ptm_engine$(EXT)
	./bin/ptm_engine$(EXT) --all

clean:
	$(RM)
