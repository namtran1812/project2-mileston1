TARGET = project2.out
SRCS = src/*.cpp 

all: $(TARGET)

$(TARGET): $(SRCS)
	g++ -std=c++11 -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) 
	rm -f output/*.tga  

tasks: $(TARGET)
	# Tasks 11-13
	./$(TARGET) output/task11.tga input/circles.tga multiply input/layer1.tga
	./$(TARGET) output/task12.tga input/layer1.tga flip
	./$(TARGET) output/task13.tga input/layer1.tga subtract input/layer2.tga
	
	# Tasks 14-16
	./$(TARGET) output/task14.tga input/car.tga flip flip flip flip
	./$(TARGET) output/task15.tga input/car.tga subtract input/layer1.tga multiply input/circles.tga flip
	./$(TARGET) output/task16.tga input/car.tga scaleblue 3 addred 100 scalegreen 0

	# Tasks 17-20 (use placeholder filenames)
	./$(TARGET) output/task17.tga input/sample1.tga your_method_here arg
	./$(TARGET) output/task18.tga input/sample2.tga your_method_here arg
	./$(TARGET) output/task19.tga input/sample3.tga your_method_here arg
	./$(TARGET) output/task20.tga input/sample4.tga your_method_here arg

.PHONY: all run clean build tasks
