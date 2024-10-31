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

task11:
	./$(TARGET) output/part11.tga input/layer3.tga add input/pattern3.tga

task12:
	./$(TARGET) output/part12.tga input/car.tga overlay input/layer4.tga

task13:
	./$(TARGET) output/part13.tga input/layer5.tga subtract input/pattern4.tga

# Additional task rules for parts 14-20 (if needed)
task14:
	./$(TARGET) output/part14.tga input/car.tga flip flip flip flip

task15:
	./$(TARGET) output/part15.tga input/car.tga subtract input/layer1.tga multiply input/circles.tga flip

task16:
	./$(TARGET) output/part16.tga input/car.tga scaleblue 3 scalered 2 scalegreen 0

task17:
	./$(TARGET) output/part17.tga input/car.tga addred 50 addblue 100 addgreen 150

task18:
	./$(TARGET) output/part18.tga input/layer5.tga scaleblue 2 combine input/layer2.tga addgreen 200

task19:
	./$(TARGET) output/part19.tga input/layer4.tga overlay input/pattern1.tga subtract input/layer1.tga

task20:
	./$(TARGET) output/part20.tga input/layer3.tga screen input/layer2.tga onlygreen

# Run all tasks sequentially for testing
tasks: task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task12 task13 task14 task15 task16 task17 task18 task19 task20

# Clean up executable and output images
clean:
	rm -f $(TARGET)
	rm -f output/*.tga  

.PHONY: all build run_all_tasks clean task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task12 task13 task14 task15 task16 task17 task18 task19 task20
