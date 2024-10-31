#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>

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
    header[17] = 0x00; // Standard image descriptor byte (bottom-left origin)

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>((p1.b * p2.b) / 255);
    result.g = static_cast<unsigned char>((p1.g * p2.g) / 255);
    result.r = static_cast<unsigned char>((p1.r * p2.r) / 255);
    return result;
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {static_cast<unsigned char>(std::max(0, p1.b - p2.b)),
            static_cast<unsigned char>(std::max(0, p1.g - p2.g)),
            static_cast<unsigned char>(std::max(0, p1.r - p2.r))};
}

void flipImage(Image& image) {
    std::reverse(image.pixels.begin(), image.pixels.end());
}

void add_channel(Image& image, int value, char channel) {
    for (Pixel& p : image.pixels) {
        if (channel == 'r') {
            p.r = static_cast<unsigned char>(std::min(255, std::max(0, p.r + value)));
        } else if (channel == 'g') {
            p.g = static_cast<unsigned char>(std::min(255, std::max(0, p.g + value)));
        } else if (channel == 'b') {
            p.b = static_cast<unsigned char>(std::min(255, std::max(0, p.b + value)));
        }
    }
}

void scale_channel(Image& image, int factor, char channel) {
    for (Pixel& p : image.pixels) {
        if (channel == 'r') p.r = static_cast<unsigned char>(std::min(255, p.r * factor));
        else if (channel == 'g') p.g = static_cast<unsigned char>(std::min(255, p.g * factor));
        else if (channel == 'b') p.b = static_cast<unsigned char>(std::min(255, p.b * factor));
    }
}

void overlay(Image& image, const Image& layer) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].b = static_cast<unsigned char>(image.pixels[i].b + layer.pixels[i].b - (image.pixels[i].b * layer.pixels[i].b) / 255);
        image.pixels[i].g = static_cast<unsigned char>(image.pixels[i].g + layer.pixels[i].g - (image.pixels[i].g * layer.pixels[i].g) / 255);
        image.pixels[i].r = static_cast<unsigned char>(image.pixels[i].r + layer.pixels[i].r - (image.pixels[i].r * layer.pixels[i].r) / 255);
    }
}

void screen(Image& image, const Image& layer) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].b = static_cast<unsigned char>(255 - ((255 - image.pixels[i].b) * (255 - layer.pixels[i].b)) / 255);
        image.pixels[i].g = static_cast<unsigned char>(255 - ((255 - image.pixels[i].g) * (255 - layer.pixels[i].g)) / 255);
        image.pixels[i].r = static_cast<unsigned char>(255 - ((255 - image.pixels[i].r) * (255 - layer.pixels[i].r)) / 255);
    }
}

void only_red(Image& image) {
    for (Pixel& p : image.pixels) {
        p.g = 0;
        p.b = 0;
    }
}

void only_green(Image& image) {
    for (Pixel& p : image.pixels) {
        p.r = 0;
        p.b = 0;
    }
}

void combine(Image& image, const Image& r, const Image& g, const Image& b) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].r = r.pixels[i].r;
        image.pixels[i].g = g.pixels[i].g;
        image.pixels[i].b = b.pixels[i].b;
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
        std::cerr << "Error: File does not exist." << std::endl;
        return 1;
    }

    int argIndex = 3;
    while (argIndex < argc) {
        std::string method = argv[argIndex];
        // Further logic for each method
        argIndex++;
    }

    if (!trackingImage.save(outputFilename)) {
        std::cerr << "Error: Could not save file " << outputFilename << std::endl;
        return 1;
    }

    std::cout << "Output saved to " << outputFilename << std::endl;
    return 0;
}
