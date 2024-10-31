#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

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
    header[2] = 2;
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24;

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Image manipulation functions
Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round((p1.b / 255.0) * (p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round((p1.g / 255.0) * (p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round((p1.r / 255.0) * (p2.r / 255.0) * 255));
    return result;
}

Pixel screen(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round(255 - (1 - p1.b / 255.0) * (1 - p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round(255 - (1 - p1.g / 255.0) * (1 - p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round(255 - (1 - p1.r / 255.0) * (1 - p2.r / 255.0) * 255));
    return result;
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {static_cast<unsigned char>(std::max(0, p1.b - p2.b)),
            static_cast<unsigned char>(std::max(0, p1.g - p2.g)),
            static_cast<unsigned char>(std::max(0, p1.r - p2.r))};
}

Pixel add(const Pixel& p1, const Pixel& p2) {
    return {static_cast<unsigned char>(std::min(255, p1.b + p2.b)),
            static_cast<unsigned char>(std::min(255, p1.g + p2.g)),
            static_cast<unsigned char>(std::min(255, p1.r + p2.r))};
}

Pixel overlay(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    auto overlay_channel = [](float np1, float np2) {
        return (np2 <= 0.5f) ? 2 * np1 * np2 : 1 - 2 * (1 - np1) * (1 - np2);
    };

    result.b = static_cast<unsigned char>(std::round(overlay_channel(p1.b / 255.0, p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round(overlay_channel(p1.g / 255.0, p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round(overlay_channel(p1.r / 255.0, p2.r / 255.0) * 255));
    return result;
}

// Command-line task functions
void performMultiply(Image &trackingImg, const std::string &filename) {
    Image otherImg;
    if (!otherImg.load(filename)) { return; }
    for (size_t i = 0; i < trackingImg.pixels.size(); ++i) {
        trackingImg.pixels[i] = multiply(trackingImg.pixels[i], otherImg.pixels[i]);
    }
}

void performScreen(Image &trackingImg, const std::string &filename) {
    Image otherImg;
    if (!otherImg.load(filename)) { return; }
    for (size_t i = 0; i < trackingImg.pixels.size(); ++i) {
        trackingImg.pixels[i] = screen(trackingImg.pixels[i], otherImg.pixels[i]);
    }
}

void performAddGreen(Image &img, int value) {
    for (auto &pixel : img.pixels) {
        pixel.g = std::min(255, pixel.g + value);
    }
}

void performScaleRed(Image &img, int value) {
    for (auto &pixel : img.pixels) {
        pixel.r = std::min(255, pixel.r * value);
    }
}

void performFlip(Image &img) {
    int width = img.width;
    int height = img.height;
    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width; ++x) {
            std::swap(img.pixels[y * width + x], img.pixels[(height - y - 1) * width + x]);
        }
    }
}

// Print help message
void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2023\n\n"
              << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
}

bool validateTGAFileName(const std::string &filename) {
    return filename.size() > 4 && filename.substr(filename.size() - 4) == ".tga";
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 3 || (argc == 2 && std::string(argv[1]) == "--help")) {
        std::cout << "Project 2: Image Processing, Fall 2024\n\n"
                  << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]\n";
        return 0;
    }

    std::string outputFilename = argv[1];
    std::string firstImageFilename = argv[2];

    // Validate output and input filenames
    if (!validateTGAFileName(outputFilename) || !validateTGAFileName(firstImageFilename)) {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    Image trackingImg;
    if (!trackingImg.load(firstImageFilename)) {
        std::cerr << "Error: Could not open file " << firstImageFilename << std::endl;
        return 1;
    }

    for (int i = 3; i < argc; ++i) {
        std::string method = argv[i];
        if (method == "multiply") {
            if (++i >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string multiplyFile = argv[i];
            if (!validateTGAFileName(multiplyFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            Image otherImg;
            if (!otherImg.load(multiplyFile)) {
                std::cerr << "Error: Could not open file " << multiplyFile << std::endl;
                return 1;
            }
            for (size_t j = 0; j < trackingImg.pixels.size(); ++j) {
                trackingImg.pixels[j] = multiply(trackingImg.pixels[j], otherImg.pixels[j]);
            }
        } else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
    }

    if (!trackingImg.save(outputFilename)) {
        std::cerr << "Error writing output file." << std::endl;
        return 1;
    }

    std::cout << "Processing completed successfully." << std::endl;
    return 0;
}
