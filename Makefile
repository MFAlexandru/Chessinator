CXX = g++
CXX_FLAGS = -Wall -std=c++17 -Wno-narrowing -O3 -funroll-loops

BIN = bin
SRC = src
INCLUDE = include
EXECUTABLE = CHESSNOT

build:
	$(CXX) $(CXX_FLAGS) $(SRC)/* -o ./$(BIN)/$(EXECUTABLE) -I ./$(INCLUDE)
	
run: build
	./$(BIN)/$(EXECUTABLE)

clean:
	-rm $(BIN)/$(EXECUTABLE)
	-rm manual_debug.txt
	-rm xboard.debug

