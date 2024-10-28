CXX = g++
CXXFLAGS = -std=c++11

project2.out: src/main.cpp src/TGAImage.cpp src/ImageManipulations.cpp
	$(CXX) $(CXXFLAGS) -o project2.out src/main.cpp src/TGAImage.cpp src/ImageManipulations.cpp

clean:
	rm -f project2.out
