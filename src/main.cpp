#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tgaimage.h"
using namespace std;

void helpMessage() {
    cout << "Project 2: Image Processing, Fall 2024\n" << endl;
    cout << "Usage:" << endl;
    cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
}

// Check if the filename ends with ".tga"
bool isValidTgaFileName(const string& name) {
    if (name.size() < 4 || name.substr(name.size() - 4) != ".tga") {
        cout << "Invalid file name." << endl;
        return false;
    }
    return true;
}

// Check if the file exists
bool doesFileExist(const string& name) {
    ifstream file(name, ios::binary);
    if (!file.is_open()) {
        cout << "File does not exist." << endl;
        return false;
    }
    return true;
}

// Assuming Picture and tgaimage functionalities are defined elsewhere
Picture trackingImage;
Picture bot;
int value;

int main(int argc, char* argv[]) {
    if (argc <= 1 || string(argv[1]) == "--help") {
        helpMessage();
        return 0;
    }

    // Validate output file name (should end with ".tga")
    if (!isValidTgaFileName(argv[1])) {
        return 0;
    }

    // Validate input file name (initial tracking image should end with ".tga" and exist)
    if (argc < 3 || !isValidTgaFileName(argv[2]) || !doesFileExist(argv[2])) {
        return 0;
    } else {
        trackingImage.readData(argv[2], trackingImage);
    }

    // Process methods and validate additional file arguments where applicable
    int index = 3;
    while (index < argc) {
        string method = argv[index];

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (index + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            if (!isValidTgaFileName(argv[index + 1]) || !doesFileExist(argv[index + 1])) {
                return 0;
            }
            bot.readData(argv[index + 1], bot);
            if (method == "multiply") trackingImage.multiply(trackingImage, bot, trackingImage);
            else if (method == "subtract") trackingImage.subtract(trackingImage, bot, trackingImage);
            else if (method == "overlay") trackingImage.overlay(trackingImage, bot, trackingImage);
            else if (method == "screen") trackingImage.screen(trackingImage, bot, trackingImage);
            cout << "Processing " << method << "..." << endl;
            index++;
        } else if (method == "combine") {
            if (index + 2 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            if (!isValidTgaFileName(argv[index + 1]) || !doesFileExist(argv[index + 1]) ||
                !isValidTgaFileName(argv[index + 2]) || !doesFileExist(argv[index + 2])) {
                return 0;
            }
            Picture green, blue;
            green.readData(argv[index + 1], green);
            blue.readData(argv[index + 2], blue);
            trackingImage.combine(trackingImage, green, blue, trackingImage);
            cout << "Combining channels..." << endl;
            index += 2;
        } else if (method == "flip" || method == "onlyred" || method == "onlygreen" || method == "onlyblue") {
            if (method == "flip") trackingImage.flip(trackingImage, trackingImage);
            else if (method == "onlyred") trackingImage.onlyred(trackingImage, trackingImage);
            else if (method == "onlygreen") trackingImage.onlygreen(trackingImage, trackingImage);
            else if (method == "onlyblue") trackingImage.onlyblue(trackingImage, trackingImage);
            cout << "Processing " << method << "..." << endl;
        } else {
            cout << "Invalid method name." << endl;
            return 0;
        }

        index++;
    }

    cout << "write" << endl;
    trackingImage.writeData(argv[1], trackingImage);
    return 0;
}
