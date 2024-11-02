
#include "TGAImage.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <cctype>

bool isTGAFile(const std::string &filename) {
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".tga";
}

void printHelp() {
    std::cout << "Usage: ./project2.out [output] [input] [method] [arguments...]\n";
    std::cout << "Supported methods:\n";
    std::cout << "  multiply [file]     - Multiply with another image\n";
    std::cout << "  subtract [file]     - Subtract another image\n";
    std::cout << "  screen [file]       - Screen with another image\n";
    std::cout << "  overlay [file]      - Overlay with another image\n";
    std::cout << "  addcolor [r g b]    - Add color values\n";
    std::cout << "  scalecolor [r g b]  - Scale color values\n";
    std::cout << "  extract [channel]   - Extract color channel (red, green, blue)\n";
    std::cout << "  combine [r g b]     - Combine separate red, green, blue images\n";
    std::cout << "  flip                - Flip image vertically\n";
}

bool validateFile(const std::string &filename, const std::string &errorMessage) {
    if (!isTGAFile(filename)) {
        std::cerr << "Invalid file name.\n";
        return false;
    }
    if (!std::filesystem::exists(filename)) {
        std::cerr << errorMessage << "\n";
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printHelp();
        return 1;
    }

    std::string outputFile = argv[1];
    std::string inputFile = argv[2];
    std::string method = argv[3];

    // Validate output file extension
    if (!isTGAFile(outputFile)) {
        std::cerr << "Invalid file name.\n";
        return 1;
    }

    // Validate input file
    if (!validateFile(inputFile, "File does not exist.")) {
        return 1;
    }

    TGAImage image;
    if (!image.readFromFile(inputFile)) {
        std::cerr << "Error: Could not open input file " << inputFile << "\n";
        return 1;
    }

    // Process the method
    if (method == "multiply" && argc == 5) {
        std::string otherFile = argv[4];
        if (!validateFile(otherFile, "Invalid argument, file does not exist.")) {
            return 1;
        }
        TGAImage other;
        other.readFromFile(otherFile);
        image.multiply(other);
    } else if (method == "subtract" && argc == 5) {
        std::string otherFile = argv[4];
        if (!validateFile(otherFile, "Invalid argument, file does not exist.")) {
            return 1;
        }
        TGAImage other;
        other.readFromFile(otherFile);
        image.subtract(other);
    } else if (method == "screen" && argc == 5) {
        std::string otherFile = argv[4];
        if (!validateFile(otherFile, "Invalid argument, file does not exist.")) {
            return 1;
        }
        TGAImage other;
        other.readFromFile(otherFile);
        image.screen(other);
    } else if (method == "overlay" && argc == 5) {
        std::string otherFile = argv[4];
        if (!validateFile(otherFile, "Invalid argument, file does not exist.")) {
            return 1;
        }
        TGAImage other;
        other.readFromFile(otherFile);
        image.overlay(other);
    } else if (method == "addcolor" && argc == 7) {
        try {
            int r = std::stoi(argv[4]);
            int g = std::stoi(argv[5]);
            int b = std::stoi(argv[6]);
            image.addColor(r, g, b);
        } catch (...) {
            std::cerr << "Invalid argument, expected number.\n";
            return 1;
        }
    } else if (method == "scalecolor" && argc == 7) {
        try {
            int r = std::stoi(argv[4]);
            int g = std::stoi(argv[5]);
            int b = std::stoi(argv[6]);
            image.scaleColor(r, g, b);
        } catch (...) {
            std::cerr << "Invalid argument, expected number.\n";
            return 1;
        }
    } else if (method == "extract" && argc == 5) {
        std::string channel = argv[4];
        if (channel != "red" && channel != "green" && channel != "blue") {
            std::cerr << "Invalid argument, expected channel name.\n";
            return 1;
        }
        TGAImage output;
        image.extractChannel(channel, output);
        output.writeToFile(outputFile);
        return 0;
    } else if (method == "combine" && argc == 7) {
        std::string redFile = argv[4], greenFile = argv[5], blueFile = argv[6];
        if (!validateFile(redFile, "Invalid argument, file does not exist.") ||
            !validateFile(greenFile, "Invalid argument, file does not exist.") ||
            !validateFile(blueFile, "Invalid argument, file does not exist.")) {
            return 1;
        }
        TGAImage red, green, blue;
        red.readFromFile(redFile);
        green.readFromFile(greenFile);
        blue.readFromFile(blueFile);
        image.combineChannels(red, green, blue);
    } else if (method == "flip") {
        image.flip();
    } else {
        std::cerr << "Invalid method name.\n";
        printHelp();
        return 1;
    }

    if (!image.writeToFile(outputFile)) {
        std::cerr << "Error: Could not write output file " << outputFile << "\n";
        return 1;
    }

    std::cout << "Output saved to " << outputFile << "\n";
    return 0;
}
