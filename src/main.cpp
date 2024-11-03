#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

bool validOutputFileName(const string& name) {
    // Ensure filename ends with ".tga" and is at least 4 characters
    return name.length() >= 4 && name.substr(name.length() - 4) == ".tga";
}

bool validFileName(const string& name) {
    // Check for ".tga" extension and file existence
    if (name.length() < 4 || name.substr(name.length() - 4) != ".tga") {
        return false;
    }
    ifstream file(name, ios::binary);
    return file.is_open();  // Confirm that the file exists
}

bool isInt(const string& value) {
    try {
        stoi(value); // Attempt to convert to int
        return true;
    } catch (const invalid_argument&) {
        return false;
    }
}

void validateArguments(int argc, char* argv[]) {
    if (argc <= 1 || string(argv[1]) == "--help") {
        cout << "Project 2: Image Processing, Fall 2024\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
        exit(0);
    }

    if (!validOutputFileName(argv[1])) {
        cerr << "Invalid output file name.\n";
        exit(1);
    }

    if (argc <= 2 || !validFileName(argv[2])) {
        cerr << "Invalid file name or file not found for initial image.\n";
        exit(1);
    }
}

class Pixel {
public:
    unsigned char blue;
    unsigned char green;
    unsigned char red;

    Pixel() : blue(0), green(0), red(0) {}
    Pixel(char b, char g, char r) : blue(b), green(g), red(r) {}
};

class Picture {
public:
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    vector<Pixel> pixels;

    Picture() : idLength(0), colorMapType(0), dataTypeCode(0), colorMapOrigin(0), colorMapLength(0), 
                colorMapDepth(0), xOrigin(0), yOrigin(0), width(0), height(0), bitsPerPixel(0), 
                imageDescriptor(0) {}

    void readData(const string& filePath) {
        ifstream file(filePath, ios::in | ios::binary);
        if (file.is_open()) {
            file.read(reinterpret_cast<char*>(&idLength), sizeof(idLength));
            file.read(reinterpret_cast<char*>(&colorMapType), sizeof(colorMapType));
            file.read(reinterpret_cast<char*>(&dataTypeCode), sizeof(dataTypeCode));
            file.read(reinterpret_cast<char*>(&colorMapOrigin), sizeof(colorMapOrigin));
            file.read(reinterpret_cast<char*>(&colorMapLength), sizeof(colorMapLength));
            file.read(reinterpret_cast<char*>(&colorMapDepth), sizeof(colorMapDepth));
            file.read(reinterpret_cast<char*>(&xOrigin), sizeof(xOrigin));
            file.read(reinterpret_cast<char*>(&yOrigin), sizeof(yOrigin));
            file.read(reinterpret_cast<char*>(&width), sizeof(width));
            file.read(reinterpret_cast<char*>(&height), sizeof(height));
            file.read(reinterpret_cast<char*>(&bitsPerPixel), sizeof(bitsPerPixel));
            file.read(reinterpret_cast<char*>(&imageDescriptor), sizeof(imageDescriptor));

            int imageSize = width * height;
            pixels.resize(imageSize);

            for (int i = 0; i < imageSize; i++) {
                file.read(reinterpret_cast<char*>(&pixels[i].blue), sizeof(pixels[i].blue));
                file.read(reinterpret_cast<char*>(&pixels[i].green), sizeof(pixels[i].green));
                file.read(reinterpret_cast<char*>(&pixels[i].red), sizeof(pixels[i].red));
            }

            file.close();
        } else {
            cout << "File not found.\n";
            exit(1);
        }
    }

    void writeData(const string& filePath) {
        ofstream file(filePath, ios::out | ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<char*>(&idLength), sizeof(idLength));
            file.write(reinterpret_cast<char*>(&colorMapType), sizeof(colorMapType));
            file.write(reinterpret_cast<char*>(&dataTypeCode), sizeof(dataTypeCode));
            file.write(reinterpret_cast<char*>(&colorMapOrigin), sizeof(colorMapOrigin));
            file.write(reinterpret_cast<char*>(&colorMapLength), sizeof(colorMapLength));
            file.write(reinterpret_cast<char*>(&colorMapDepth), sizeof(colorMapDepth));
            file.write(reinterpret_cast<char*>(&xOrigin), sizeof(xOrigin));
            file.write(reinterpret_cast<char*>(&yOrigin), sizeof(yOrigin));
            file.write(reinterpret_cast<char*>(&width), sizeof(width));
            file.write(reinterpret_cast<char*>(&height), sizeof(height));
            file.write(reinterpret_cast<char*>(&bitsPerPixel), sizeof(bitsPerPixel));
            file.write(reinterpret_cast<char*>(&imageDescriptor), sizeof(imageDescriptor));

            int imageSize = width * height;
            for (int i = 0; i < imageSize; i++) {
                file.write(reinterpret_cast<char*>(&pixels[i].blue), sizeof(pixels[i].blue));
                file.write(reinterpret_cast<char*>(&pixels[i].green), sizeof(pixels[i].green));
                file.write(reinterpret_cast<char*>(&pixels[i].red), sizeof(pixels[i].red));
            }

            file.close();
        } else {
            cout << "Unable to open output file.\n";
        }
    }
};

int main(int argc, char* argv[]) {
    // Argument validation before proceeding
    validateArguments(argc, argv);

    Picture trackingImage;
    trackingImage.readData(argv[2]);

    int index = 3;
    while (index < argc) {
        string method = argv[index];

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (index + 1 >= argc) {
                cout << "Missing argument for " << method << ".\n";
                return 1;
            }
            if (!validFileName(argv[index + 1])) {
                cout << "Invalid argument, file does not exist.\n";
                return 1;
            }
            // Method processing would go here
            index += 2;
        }
        else if (method == "addred" || method == "addgreen" || method == "addblue" ||
                 method == "scalered" || method == "scalegreen" || method == "scaleblue") {
            if (index + 1 >= argc || !isInt(argv[index + 1])) {
                cout << "Invalid argument, expected a number for " << method << ".\n";
                return 1;
            }
            // Method processing would go here
            index += 2;
        }
        else if (method == "combine" || method == "flip" || method == "onlyred" ||
                 method == "onlygreen" || method == "onlyblue") {
            // Special cases for other commands
            // Method processing would go here
            index++;
        }
        else {
            cout << "Invalid method name: " << method << "\n";
            return 1;
        }
    }

    trackingImage.writeData(argv[1]);
    cout << "write\n";
    return 0;
}
