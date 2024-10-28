# Makefile for Image Processing Project - Milestone 1

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Source and output files
SRC = main.cpp
EXEC = project2.out

# Default target to build the project
all: $(EXEC)

# Link and compile
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

# Clean up build files
clean:
	rm -f $(EXEC) *.o

# Run the executable
run: $(EXEC)
	./$(EXEC)

# Phony targets
.PHONY: all clean run
