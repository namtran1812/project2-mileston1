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
    if (name.length() < 4) return false;  // Check if filename has at least four characters
    string lastFourCharacters = name.substr(name.length() - 4);
    return lastFourCharacters == ".tga";
}

bool validFileName(const string& name) {
    if (name.length() < 4) return false;  // Check if filename has at least four characters
    string lastFourCharacters = name.substr(name.length() - 4);
    if (lastFourCharacters != ".tga") {
        return false;
    }
    ifstream file(name, ios::binary);
    return file.is_open(); // Verify the file can be opened
}

bool isInt(string value) {
    try {
        value = stoi(value);
    }
    catch (invalid_argument) {
        return false;
    }
    return true;

}


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

    if (!validFileName(argv[1])) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (argc <= 2 || !validOutputFileName(argv[2])) {
        cout << "Invalid file name." << endl;
        return 0;
    }
        else {
            trackingImage.readData(argv[2], trackingImage);
        }

    int index = 3;
    while (index < argc) {
        string method = argv[index];
        cout << argv[index] << endl;

        if (string(argv[index]).size() >= 5 && !validFileName(argv[index]) && (method != "multiply" && method != "subtract" && method != "overlay" &&
        method != "screen" && method != "combine" && method != "flip" &&
        method != "onlyred" && method != "onlygreen" && method != "onlyblue" &&
        method != "addred" && method != "addgreen" && method != "addblue" &&
        method != "scalered" && method != "scalegreen" && method != "scaleblue")) {
            cout << "Invalid method name." << endl;
            return 0;
        }

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (index + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            if (!validFileName(argv[index + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (method == "multiply") {
                bot.readData(argv[index + 1], bot);
                trackingImage.multiply(trackingImage, bot, trackingImage);
                cout << "Multiplying " << argv[index - 1] << " and " << argv[index + 1] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "subtract") {
                bot.readData(argv[index + 1], bot);
                trackingImage.subtract(trackingImage, bot, trackingImage);

                cout << "Subtracting " << argv[index - 1] << " and " << argv[index + 1] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "overlay") {
                bot.readData(argv[index + 1], bot);
                trackingImage.overlay(trackingImage, bot, trackingImage);

                cout << "Overlaying " << argv[index - 1] << "and " << argv[index + 1] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "screen") {
                bot.readData(argv[index + 1], bot);
                trackingImage.screen(trackingImage, bot, trackingImage);
                cout << "Screening " << argv[index - 1] << " and " << argv[index + 1] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
        }

        if (method == "combine") {
            if (index + 1 >= argc || index + 2 >= argc) {
                cout << "Missing argument." << endl;
                return 0;
            }
            if (!validFileName(argv[index + 1]) || !validFileName(argv[index + 2])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }
            // red is tracking image
            blue.readData(argv[index + 2], blue);
            green.readData(argv[index + 1], green);

            trackingImage.combine(trackingImage, green, blue, trackingImage);

            cout << "Combining " << argv[index - 1] << " and " << argv[index + 1] << " and " << argv[index + 2] << "..." << endl;
            cout << "... and saving output to " << argv[1] << "!";
            index ++;
        }

        if (method == "flip" || method == "onlyred" || method == "onlygreen" || method == "onlyblue") {
            if (method == "flip") {
                trackingImage.flip(trackingImage, trackingImage);

                cout << "Flipping " << argv[2] << " and " << argv[4] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "onlyred") {
                trackingImage.onlyred(trackingImage, trackingImage);

                cout << "Onlyredding " << argv[2] << " and " << argv[4] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "onlygreen") {
                trackingImage.onlygreen(trackingImage, trackingImage);

                cout << "Onlygreening " << argv[2] << " and " << argv[4] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "onlyblue") {
                trackingImage.onlyblue(trackingImage, trackingImage);

                cout << "Onlyblueing " << argv[2] << "and " << argv[4] << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
        }

        if (method == "addred" || method == "addgreen" || method == "addblue" || method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (index + 1 >= argc) {
                cout << "Missing argument." << endl;
                return 1;
            }
            if (!isInt(argv[index + 1])) {
                cout << "Invalid argument, expected number." << endl;
                return 1;
            }
            if (method == "addred") {
                trackingImage.addred(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Adding +" << stoi(argv[index + 1]) << " to red channel " << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "addgreen") {
                trackingImage.addgreen(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Adding +" << stoi(argv[index + 1]) << " to green channel " << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "addblue") {
                trackingImage.addblue(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Adding +" << stoi(argv[index + 1]) << " to blue channel " << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "scalered") {
                trackingImage.scalered(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Scaling the red channel by " << stoi(argv[index + 1]) << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "scalegreen") {
                trackingImage.scalegreen(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Scaling the green channel by " << stoi(argv[index + 1]) << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
            else if (method == "scaleblue") {
                trackingImage.scaleblue(trackingImage, stoi(argv[index + 1]), trackingImage);

                cout << "Scaling the blue channel by " << stoi(argv[index + 1]) << "..." << endl;
                cout << "... and saving output to " << argv[1] << "!";
            }
        }
        index ++;
    }
    cout << "write"<< endl;
    trackingImage.writeData(argv[1], trackingImage);
    return 0;
}
