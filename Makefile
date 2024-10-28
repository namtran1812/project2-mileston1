TARGET = project2.out
SRCS = src/*.cpp 

all: $(TARGET)

$(TARGET): $(SRCS)
	g++ -std=c++11 -o $(TARGET) $(SRCS)

build: $(SRCS)
	g++ -std=c++11 -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) 
	rm -f output/*.tga  

.PHONY: all run clean build
