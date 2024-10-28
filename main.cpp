#include "TGAImage.h"
#include "ImageManipulations.h"
#include <iostream>

int main() {
    TGAImage layer1, pattern1, car, text, result;

    // Task 1: Multiply
    if (layer1.load("input/layer1.tga") && pattern1.load("input/pattern1.tga")) {
        multiply(layer1, pattern1);
        layer1.save("output/part1.tga");
    }

    // Task 2: Subtract
    TGAImage layer2;
    if (layer2.load("input/layer2.tga") && car.load("input/car.tga")) {
        subtract(car, layer2);
        car.save("output/part2.tga");
    }

    // Task 3: Screen
    if (layer1.load("input/layer1.tga") && pattern1.load("input/pattern2.tga") && text.load("input/text.tga")) {
        multiply(layer1, pattern1);
        screen(layer1, text);
        layer1.save("output/part3.tga");
    }

    // Continue with similar implementations for each task...

    return 0;
}
