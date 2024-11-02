#include <iostream>
#include <string>
#include <vector>
#include "Image.h"

// Function to print the help message
void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2024\n\n"
              << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

// Command-line image processing
bool processCommand(const std::string& method, Image& trackingImage, const std::vector<std::string>& args) {
    if (method == "multiply" && args.size() == 1) {
        Image other(args[0]);
        trackingImage.multiply(other);
    } else if (method == "subtract" && args.size() == 1) {
        Image other(args[0]);
        trackingImage.subtract(other);
    } else if (method == "flip" && args.empty()) {
        trackingImage.flip();
    } else if (method == "onlyred" && args.empty()) {
        trackingImage.onlyRed();
    } else if (method == "onlygreen" && args.empty()) {
        trackingImage.onlyGreen();
    } else {
        std::cerr << "Invalid method or incorrect arguments.\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || (argc == 2 && std::string(argv[1]) == "--help")) {
        printHelp();
        return 0;
    }

    std::string outputFile = argv[1];
    if (outputFile.find(".tga") == std::string::npos) {
        std::cerr << "Invalid file name.\n";
        return 1;
    }

    std::string sourceFile = argv[2];
    Image trackingImage(sourceFile);

    for (int i = 3; i < argc; i++) {
        std::string method = argv[i];
        std::vector<std::string> methodArgs;

        if (method == "multiply" || method == "subtract") {
            if (++i < argc) methodArgs.push_back(argv[i]);
        }

        if (!processCommand(method, trackingImage, methodArgs)) return 1;
    }

    if (!trackingImage.save(outputFile)) {
        std::cerr << "Failed to save image to " << outputFile << "\n";
        return 1;
    }

    std::cout << "Output saved to " << outputFile << "\n";
    return 0;
}
