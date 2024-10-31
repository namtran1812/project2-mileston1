TARGET = project2.out
SRCS = src/*.cpp 

# Default rule to build the project
all: $(TARGET)

# Compile the project
$(TARGET): $(SRCS)
	g++ -std=c++11 -o $(TARGET) $(SRCS)

# Build command
build: $(SRCS)
	g++ -std=c++11 -o $(TARGET) $(SRCS)

# Run specific tasks for Milestone 2
# Each rule here corresponds to a task using command-line arguments
task1:
	./$(TARGET) output/part1.tga input/layer1.tga multiply input/pattern1.tga

task2:
	./$(TARGET) output/part2.tga input/layer2.tga subtract input/car.tga

task3:
	./$(TARGET) output/part3.tga input/layer1.tga multiply input/pattern2.tga screen input/text.tga

task4:
	./$(TARGET) output/part4.tga input/layer2.tga multiply input/circles.tga subtract input/pattern2.tga

task5:
	./$(TARGET) output/part5.tga input/layer1.tga overlay input/pattern1.tga

task6:
	./$(TARGET) output/part6.tga input/car.tga addgreen 200

task7:
	./$(TARGET) output/part7.tga input/car.tga scaleRed 4 scaleBlue 0

task8:
	./$(TARGET) output/part8_r.tga input/car.tga onlyred
	./$(TARGET) output/part8_g.tga input/car.tga onlygreen
	./$(TARGET) output/part8_b.tga input/car.tga onlyblue

task9:
	./$(TARGET) output/part9.tga input/layer_red.tga combine input/layer_green.tga input/layer_blue.tga

task10:
	./$(TARGET) output/part10.tga input/text2.tga flip

# Run all tasks sequentially for testing
run_all_tasks: task1 task2 task3 task4 task5 task6 task7 task8 task9 task10

# Clean up executable and output images
clean:
	rm -f $(TARGET)
	rm -f output/*.tga  

.PHONY: all build run_all_tasks clean task1 task2 task3 task4 task5 task6 task7 task8 task9 task10
