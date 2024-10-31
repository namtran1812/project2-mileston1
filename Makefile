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
	./$(TARGET) output/part11.tga input/circles.tga multiply input/layer1.tga
	./$(TARGET) output/part12.tga input/layer1.tga flip
	./$(TARGET) output/part13.tga input/layer1.tga subtract input/layer2.tga
	
	# Tasks 14-20
	./$(TARGET) output/part14.tga input/car.tga flip flip flip flip
	./$(TARGET) output/part15.tga input/car.tga subtract input/layer1.tga multiply input/circles.tga flip
	./$(TARGET) output/part16.tga input/car.tga scaleblue 3 addred 100 scalegreen 0
	./$(TARGET) output/part17.tga input/car.tga addred 30 addgreen 20 addblue 10
	./$(TARGET) output/part18.tga input/car.tga scaleblue 2 combine input/layer1.tga input/layer2.tga input/layer3.tga
	./$(TARGET) output/part19.tga input/car.tga overlay input/layer2.tga subtract input/layer1.tga
	./$(TARGET) output/part20.tga input/car.tga screen input/layer1.tga onlygreen

.PHONY: all run clean build tasks
