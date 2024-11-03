#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "tgaimage.h"
using namespace std;

void helpMessage() {
    cout << "Project 2: Image Processing, Fall 2024\n" << endl;
    cout << "Usage:" << endl;
    cout << "\t./project2.out [output] [firstImage] [method] [...]" << endl;
}

bool validOutputFileName(const string& name) {
    return name.size() >= 4 && name.substr(name.size() - 4) == ".tga";
}

bool validFileName(const string& name) {
    if (name.size() < 4 || name.substr(name.size() - 4) != ".tga") return false;
    ifstream file(name, ios::binary);
    return file.is_open();
}

bool fileExists(const string& name) {
    ifstream file(name, ios::binary);
    return file.is_open();
}

bool isInt(const string& value) {
    try {
        stoi(value);
        return true;
    } catch (invalid_argument&) {
        return false;
    }
}

// Assuming Picture and tgaimage functionalities are defined elsewhere
Picture trackingImage;
Picture top;
Picture bot;
Picture outcome;
Picture red;
Picture blue;
Picture green;
Picture first;
int value;

int main(int argc, char* argv[]) {
    if (argc <= 1 || string(argv[1]) == "--help") {
        helpMessage();
        return 0;
    }

    if (!validOutputFileName(argv[1])) {
        cout << "Invalid output file name." << endl;
        return 0;
    }

    if (!validFileName(argv[2]) || !fileExists(argv[2])) {
        cout << "Invalid argument, file does not exist." << endl;
        return 0;
    } else {
        trackingImage.readData(argv[2], trackingImage);
    }

    int index = 3;
    while (index < argc) {
        string method = argv[index];
        if (method.size() >= 5 && !validFileName(argv[index]) &&
            method != "multiply" && method != "subtract" && method != "overlay" &&
            method != "screen" && method != "combine" && method != "flip" &&
            method != "onlyred" && method != "onlygreen" && method != "onlyblue" &&
            method != "addred" && method != "addgreen" && method != "addblue" &&
            method != "scalered" && method != "scalegreen" && method != "scaleblue") {
            cout << "Invalid method name." << endl;
            return 0;
        }

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (index + 1 >= argc || !fileExists(argv[index + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            bot.readData(argv[index + 1], bot);
            if (method == "multiply") trackingImage.multiply(trackingImage, bot, trackingImage);
            else if (method == "subtract") trackingImage.subtract(trackingImage, bot, trackingImage);
            else if (method == "overlay") trackingImage.overlay(trackingImage, bot, trackingImage);
            else if (method == "screen") trackingImage.screen(trackingImage, bot, trackingImage);
            cout << "Processing " << method << " with " << argv[index - 1] << " and " << argv[index + 1] << "..." << endl;
            cout << "... and saving output to " << argv[1] << "!" << endl;
            index++;
        }

        if (method == "combine") {
            if (index + 2 >= argc || !fileExists(argv[index + 1]) || !fileExists(argv[index + 2])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }
            green.readData(argv[index + 1], green);
            blue.readData(argv[index + 2], blue);
            trackingImage.combine(trackingImage, green, blue, trackingImage);
            cout << "Combining channels..." << endl;
            cout << "... and saving output to " << argv[1] << "!" << endl;
            index += 2;
        }

        if (method == "flip" || method == "onlyred" || method == "onlygreen" || method == "onlyblue") {
            if (method == "flip") trackingImage.flip(trackingImage, trackingImage);
            else if (method == "onlyred") trackingImage.onlyred(trackingImage, trackingImage);
            else if (method == "onlygreen") trackingImage.onlygreen(trackingImage, trackingImage);
            else if (method == "onlyblue") trackingImage.onlyblue(trackingImage, trackingImage);
            cout << "Processing " << method << "..." << endl;
            cout << "... and saving output to " << argv[1] << "!" << endl;
        }

        if (method == "addred" || method == "addgreen" || method == "addblue" || method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (index + 1 >= argc || !isInt(argv[index + 1])) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }
            int value = stoi(argv[index + 1]);
            if (method == "addred") trackingImage.addred(trackingImage, value, trackingImage);
            else if (method == "addgreen") trackingImage.addgreen(trackingImage, value, trackingImage);
            else if (method == "addblue") trackingImage.addblue(trackingImage, value, trackingImage);
            else if (method == "scalered") trackingImage.scalered(trackingImage, value, trackingImage);
            else if (method == "scalegreen") trackingImage.scalegreen(trackingImage, value, trackingImage);
            else if (method == "scaleblue") trackingImage.scaleblue(trackingImage, value, trackingImage);
            cout << "Adjusting channel with " << method << " by " << value << "..." << endl;
            cout << "... and saving output to " << argv[1] << "!" << endl;
            index++;
        }
        index++;
    }

    cout << "write" << endl;
    trackingImage.writeData(argv[1], trackingImage);
    return 0;
}
