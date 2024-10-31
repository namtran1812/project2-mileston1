#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>  // For strcmp
#include <cstdlib>  // For std::stoi

struct Pixel {
    unsigned char b, g, r;
};

struct Image {
    int width, height;
    std::vector<Pixel> pixels;

    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
};

bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
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

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Manipulation Functions
Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round((p1.b / 255.0) * (p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round((p1.g / 255.0) * (p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round((p1.r / 255.0) * (p2.r / 255.0) * 255));
    return result;
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {std::max(0, p1.b - p2.b), std::max(0, p1.g - p2.g), std::max(0, p1.r - p2.r)};
}

void flipImage(Image& image) {
    std::reverse(image.pixels.begin(), image.pixels.end());
}

void add_red_channel(Image& image, int value) {
    for (Pixel& p : image.pixels) {
        int newRed = p.r + value;
        p.r = static_cast<unsigned char>(std::min(255, std::max(0, newRed)));
    }
}

void scale_blue_channel(Image& image, int factor) {
    for (Pixel& p : image.pixels) {
        p.b = static_cast<unsigned char>(std::min(255, p.b * factor));
    }
}

void scale_green_channel(Image& image, int factor) {
    for (Pixel& p : image.pixels) {
        p.g = static_cast<unsigned char>(std::min(255, p.g * factor));
    }
}

void scale_red_channel(Image& image, int factor) {
    for (Pixel& p : image.pixels) {
        p.r = static_cast<unsigned char>(std::min(255, p.r * factor));
    }
}

void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2024\n"
              << "\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        printHelp();
        return 0;
    }

    std::string outputFilename = argv[1];
    if (outputFilename.size() < 4 || outputFilename.substr(outputFilename.size() - 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    if (argc < 3) {
        std::cerr << "Missing argument for input filename." << std::endl;
        return 1;
    }
    
    std::string inputFilename = argv[2];
    if (inputFilename.size() < 4 || inputFilename.substr(inputFilename.size() - 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    Image trackingImage;
    if (!trackingImage.load(inputFilename)) {
        std::cerr << "File does not exist." << std::endl;
        return 1;
    }

    int argIndex = 3;
    while (argIndex < argc) {
        std::string method = argv[argIndex];

        if (method == "multiply") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string secondImageFile = argv[++argIndex];
            if (secondImageFile.size() < 4 || secondImageFile.substr(secondImageFile.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }
            Image secondImage;
            if (!secondImage.load(secondImageFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            for (size_t i = 0; i < trackingImage.pixels.size(); ++i) {
                trackingImage.pixels[i] = multiply(trackingImage.pixels[i], secondImage.pixels[i]);
            }
        }
        else if (method == "subtract") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string secondImageFile = argv[++argIndex];
            if (secondImageFile.size() < 4 || secondImageFile.substr(secondImageFile.size() - 4) != ".tga") {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }
            Image secondImage;
            if (!secondImage.load(secondImageFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            for (size_t i = 0; i < trackingImage.pixels.size(); ++i) {
                trackingImage.pixels[i] = subtract(trackingImage.pixels[i], secondImage.pixels[i]);
            }
        }
        else if (method == "flip") {
            flipImage(trackingImage);
        }
        else if (method == "addred") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int value = std::stoi(argv[++argIndex]);
                add_red_channel(trackingImage, value);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "scaleblue") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int factor = std::stoi(argv[++argIndex]);
                scale_blue_channel(trackingImage, factor);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "scalegreen") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int factor = std::stoi(argv[++argIndex]);
                scale_green_channel(trackingImage, factor);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "scalered") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int factor = std::stoi(argv[++argIndex]);
                scale_red_channel(trackingImage, factor);
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
