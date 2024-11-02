#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm> // Added for std::all_of
#include "Image.h"   // Assuming Image class is defined for TGA files and manipulations

// Prints the help message
void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2024\n\n"
              << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

// Function to check if file exists
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Function to check if a string is a number
bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// Handles image manipulations
bool handleManipulation(const std::string& method, Image& trackingImage, const std::vector<std::string>& args) {
    try {
        if (method == "multiply" && args.size() == 1) {
            Image secondImage(args[0]);
            trackingImage.multiply(secondImage);
        } else if (method == "subtract" && args.size() == 1) {
            Image secondImage(args[0]);
            trackingImage.subtract(secondImage);
        } else if (method == "overlay" && args.size() == 1) {
            Image secondImage(args[0]);
            trackingImage.overlay(secondImage);
        } else if (method == "screen" && args.size() == 1) {
            Image secondImage(args[0]);
            trackingImage.screen(secondImage);
        } else if (method == "flip" && args.empty()) {
            trackingImage.flip();
        } else if (method == "onlyred" && args.empty()) {
            trackingImage.onlyRed();
        } else if (method == "onlygreen" && args.empty()) {
            trackingImage.onlyGreen();
        } else if (method == "onlyblue" && args.empty()) {
            trackingImage.onlyBlue();
        } else if ((method == "addred" || method == "addgreen" || method == "addblue" ||
                    method == "scalered" || method == "scalegreen" || method == "scaleblue") && args.size() == 1) {
            if (!isNumber(args[0])) {
                std::cerr << "Invalid argument, expected number.\n";
                return false;
            }
            int value = std::stoi(args[0]);
            if (method == "addred") trackingImage.addRed(value);
            else if (method == "addgreen") trackingImage.addGreen(value);
            else if (method == "addblue") trackingImage.addBlue(value);
            else if (method == "scalered") trackingImage.scaleRed(value);
            else if (method == "scalegreen") trackingImage.scaleGreen(value);
            else if (method == "scaleblue") trackingImage.scaleBlue(value);
        } else {
            std::cerr << "Invalid method name.\n";
            return false;
        }
    } catch (std::exception& e) {
        std::cerr << "Error processing method " << method << ": " << e.what() << "\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Handle no arguments or --help
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        printHelp();
        return 0;
    }

    // Validate output file name
    std::string outputFile = argv[1];
    if (outputFile.find(".tga") == std::string::npos) {
        std::cerr << "Invalid file name.\n";
        return 1;
    }

    // Validate input (source) file name
    std::string sourceFile = argv[2];
    if (sourceFile.find(".tga") == std::string::npos) {
        std::cerr << "Invalid file name.\n";
        return 1;
    }
    if (!fileExists(sourceFile)) {
        std::cerr << "File does not exist.\n";
        return 1;
    }

    // Load the initial image
    Image trackingImage;
    if (!trackingImage.load(sourceFile)) {
        std::cerr << "File does not exist.\n";
        return 1;
    }

    // Process each manipulation method in order
    int i = 3;
    while (i < argc) {
        std::string method = argv[i++];
        std::vector<std::string> methodArgs;

        // Collect required arguments for each method
        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (i < argc && std::string(argv[i]).find(".tga") != std::string::npos && fileExists(argv[i])) {
                methodArgs.push_back(argv[i++]);
            } else {
                std::cerr << "Missing argument.\n";
                return 1;
            }
        } else if (method == "addred" || method == "addgreen" || method == "addblue" ||
                   method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (i < argc && isNumber(argv[i])) {
                methodArgs.push_back(argv[i++]);
            } else {
                std::cerr << "Invalid argument, expected number.\n";
                return 1;
            }
        } else if (method != "flip" && method != "onlyred" && method != "onlygreen" && method != "onlyblue") {
            std::cerr << "Invalid method name.\n";
            return 1;
        }

        // Execute the manipulation
        if (!handleManipulation(method, trackingImage, methodArgs)) {
            return 1;
        }
    }

    // Save the final output image
    if (!trackingImage.save(outputFile)) {
        std::cerr << "Failed to save the output image.\n";
        return 1;
    }

    std::cout << "Output saved to " << outputFile << "!\n";
    return 0;
}
