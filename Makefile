# Define the compiler and standard
CXX = g++
CXXFLAGS = -std=c++11

# Define the target executable
TARGET = project2.out

# Define the source files explicitly without using wildcards
SOURCES = src/main.cpp src/TGAImage.cpp src/ImageManipulations.cpp

# Rule to build the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
