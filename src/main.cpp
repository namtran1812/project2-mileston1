#include "TGAImage.h"
#include <iostream>
#include <string>

void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2023\n\n";
    std::cout << "Usage:\n\t./project2.out [output] [input] [method] [...]\n\n";
    std::cout << "Methods:\n";
    std::cout << "  multiply [file]     : Multiply with another image\n";
    std::cout << "  subtract [file]     : Subtract another image\n";
    std::cout << "  screen [file]       : Screen with another image\n";
    std::cout << "  overlay [file]      : Overlay with another image\n";
    std::cout << "  addcolor [r g b]    : Add color to channels\n";
    std::cout << "  scalecolor [r g b]  : Scale color channels\n";
    std::cout << "  extract [channel]   : Extract color channel (red, green, blue)\n";
    std::cout << "  combine [r g b]     : Combine separate red, green, blue images\n";
    std::cout << "  flip                : Flip image vertically\n";
}

int main(int argc, char *argv[]) {
    if (argc < 4 || std::string(argv[1]) == "--help") {
        printHelp();
        return 0;
    }

    std::string outputFile = argv[1];
    std::string inputFile = argv[2];
    std::string method = argv[3];

    TGAImage image;
    if (!image.readFromFile(inputFile)) {
        std::cerr << "Error: Could not open input file " << inputFile << "\n";
        return 1;
    }

    if (method == "multiply" && argc == 5) {
        TGAImage other;
        if (other.readFromFile(argv[4])) {
            image.multiply(other);
        } else {
            std::cerr << "Error: Could not open file " << argv[4] << "\n";
            return 1;
        }
    } else if (method == "subtract" && argc == 5) {
        TGAImage other;
        if (other.readFromFile(argv[4])) {
            image.subtract(other);
        } else {
            std::cerr << "Error: Could not open file " << argv[4] << "\n";
            return 1;
        }
    } else if (method == "screen" && argc == 5) {
        TGAImage other;
        if (other.readFromFile(argv[4])) {
            image.screen(other);
        } else {
            std::cerr << "Error: Could not open file " << argv[4] << "\n";
            return 1;
        }
    } else if (method == "overlay" && argc == 5) {
        TGAImage other;
        if (other.readFromFile(argv[4])) {
            image.overlay(other);
        } else {
            std::cerr << "Error: Could not open file " << argv[4] << "\n";
            return 1;
        }
    } else if (method == "addcolor" && argc == 7) {
        int r = std::stoi(argv[4]);
        int g = std::stoi(argv[5]);
        int b = std::stoi(argv[6]);
        image.addColor(r, g, b);
    } else if (method == "scalecolor" && argc == 7) {
        int r = std::stoi(argv[4]);
        int g = std::stoi(argv[5]);
        int b = std::stoi(argv[6]);
        image.scaleColor(r, g, b);
    } else if (method == "extract" && argc == 5) {
        TGAImage output;
        image.extractChannel(argv[4], output);
        if (!output.writeToFile(outputFile)) {
            std::cerr << "Error: Could not write output file " << outputFile << "\n";
            return 1;
        }
        return 0;
    } else if (method == "combine" && argc == 7) {
        TGAImage red, green, blue;
        if (red.readFromFile(argv[4]) && green.readFromFile(argv[5]) && blue.readFromFile(argv[6])) {
            image.combineChannels(red, green, blue);
        } else {
            std::cerr << "Error: Could not open one or more channel files\n";
            return 1;
        }
    } else if (method == "flip") {
        image.flip();
    } else {
        std::cerr << "Error: Invalid method or arguments\n";
        printHelp();
        return 1;
    }

    if (!image.writeToFile(outputFile)) {
        std::cerr << "Error: Could not write output file " << outputFile << "\n";
        return 1;
    }

    std::cout << "Output saved to " << outputFile << std::endl;
    return 0;
}
