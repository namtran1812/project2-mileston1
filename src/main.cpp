#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

// Clamp function for C++11 compatibility
inline int clamp(int value, int minVal, int maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

// Struct to represent a pixel with RGB channels
struct Pixel {
    unsigned char b, g, r;
};

// Struct to represent an image, including pixel data and dimensions
struct Image {
    int width, height;
    std::vector<Pixel> pixels;

    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
};

// Loads a .tga file into the Image struct
bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "File does not exist." << std::endl;
        return false;
    }

    unsigned char header[18];
    file.read(reinterpret_cast<char*>(header), sizeof(header));

    width = header[12] | (header[13] << 8);
    height = header[14] | (header[15] << 8);

    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Invalid image dimensions in " << filename << std::endl;
        return false;
    }

    pixels.resize(width * height);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to read pixel data in " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Saves the Image struct as a .tga file
bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing " << filename << std::endl;
        return false;
    }

    unsigned char header[18] = {0};
    header[2] = 2; // Uncompressed true-color image
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24; // 24 bits per pixel (RGB)
    header[17] = 0x20; // Image descriptor byte, sets origin in upper-left

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Utility function to check if a file has a .tga extension and exists
bool isValidTGAFile(const std::string& filename) {
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return false;
    }
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "File does not exist." << std::endl;
        return false;
    }
    return true;
}

// Pixel manipulation functions
Pixel multiply(const Pixel& p1, const Pixel& p2) {
    return {static_cast<unsigned char>((p1.b * p2.b) / 255),
            static_cast<unsigned char>((p1.g * p2.g) / 255),
            static_cast<unsigned char>((p1.r * p2.r) / 255)};
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {static_cast<unsigned char>(std::max(0, p1.b - p2.b)),
            static_cast<unsigned char>(std::max(0, p1.g - p2.g)),
            static_cast<unsigned char>(std::max(0, p1.r - p2.r))};
}

void overlay(Image& image, const Image& layer) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i] = multiply(image.pixels[i], layer.pixels[i]);
    }
}

void add_channel(Image& image, int value, char channel) {
    for (auto& pixel : image.pixels) {
        if (channel == 'r') pixel.r = clamp(pixel.r + value, 0, 255);
        else if (channel == 'g') pixel.g = clamp(pixel.g + value, 0, 255);
        else if (channel == 'b') pixel.b = clamp(pixel.b + value, 0, 255);
    }
}

void scale_channel(Image& image, int factor, char channel) {
    for (auto& pixel : image.pixels) {
        if (channel == 'r') pixel.r = std::min(pixel.r * factor, 255);
        else if (channel == 'g') pixel.g = std::min(pixel.g * factor, 255);
        else if (channel == 'b') pixel.b = std::min(pixel.b * factor, 255);
    }
}

// Prints help message for command-line usage
void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2024\n"
              << "\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

// Main function to process command-line arguments and perform operations
int main(int argc, char* argv[]) {
    if (argc < 2 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        printHelp();
        return 0;
    }

    std::string outputFilename = argv[1];
    if (!isValidTGAFile(outputFilename)) {
        return 1;
    }

    if (argc < 3) {
        std::cerr << "Missing argument for input filename." << std::endl;
        return 1;
    }

    std::string inputFilename = argv[2];
    if (!isValidTGAFile(inputFilename)) {
        return 1;
    }

    Image trackingImage;
    if (!trackingImage.load(inputFilename)) {
        return 1;
    }

    int argIndex = 3;
    while (argIndex < argc) {
        std::string method = argv[argIndex];

        if (method == "multiply" || method == "subtract" || method == "overlay") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string secondImageFile = argv[++argIndex];
            if (!isValidTGAFile(secondImageFile)) {
                return 1;
            }
            Image secondImage;
            if (!secondImage.load(secondImageFile)) {
                return 1;
            }

            if (method == "multiply") {
                for (size_t i = 0; i < trackingImage.pixels.size(); ++i) {
                    trackingImage.pixels[i] = multiply(trackingImage.pixels[i], secondImage.pixels[i]);
                }
            } else if (method == "subtract") {
                for (size_t i = 0; i < trackingImage.pixels.size(); ++i) {
                    trackingImage.pixels[i] = subtract(trackingImage.pixels[i], secondImage.pixels[i]);
                }
            } else if (method == "overlay") {
                overlay(trackingImage, secondImage);
            }
        }
        else if (method == "addred" || method == "addgreen" || method == "addblue") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int value = std::stoi(argv[++argIndex]);
                add_channel(trackingImage, value, method.back());
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "scaleblue" || method == "scalegreen" || method == "scalered") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int factor = std::stoi(argv[++argIndex]);
                scale_channel(trackingImage, factor, method.back());
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
        ++argIndex;
    }

    if (!trackingImage.save(outputFilename)) {
        std::cerr << "Error: Could not save file " << outputFilename << std::endl;
        return 1;
    }

    std::cout << "Output saved to " << outputFilename << std::endl;
    return 0;
}
