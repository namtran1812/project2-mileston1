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
    header[11] = 0x00;
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24; // 24 bits per pixel (RGB)
    header[17] = 0x20; // Image descriptor byte, sets origin in lower-left

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        std::cerr << "Error: Failed to write pixel data to " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Manipulation Functions (Additions & Modifications based on errors)
Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round((p1.b / 255.0) * (p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round((p1.g / 255.0) * (p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round((p1.r / 255.0) * (p2.r / 255.0) * 255));
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

// Updated scale functions
void scale_channel(Image& image, int factor, char channel) {
    for (Pixel& p : image.pixels) {
        if (channel == 'r') p.r = static_cast<unsigned char>(std::min(255, p.r * factor));
        else if (channel == 'g') p.g = static_cast<unsigned char>(std::min(255, p.g * factor));
        else if (channel == 'b') p.b = static_cast<unsigned char>(std::min(255, p.b * factor));
    }
}

void overlay(Image& image, const Image& layer) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].b = static_cast<unsigned char>((image.pixels[i].b / 255.0) * (layer.pixels[i].b / 255.0) * 255);
        image.pixels[i].g = static_cast<unsigned char>((image.pixels[i].g / 255.0) * (layer.pixels[i].g / 255.0) * 255);
        image.pixels[i].r = static_cast<unsigned char>((image.pixels[i].r / 255.0) * (layer.pixels[i].r / 255.0) * 255);
    }
}

void screen(Image& image, const Image& layer) {
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].b = static_cast<unsigned char>(255 - (1 - image.pixels[i].b / 255.0) * (1 - layer.pixels[i].b / 255.0) * 255);
        image.pixels[i].g = static_cast<unsigned char>(255 - (1 - image.pixels[i].g / 255.0) * (1 - layer.pixels[i].g / 255.0) * 255);
        image.pixels[i].r = static_cast<unsigned char>(255 - (1 - image.pixels[i].r / 255.0) * (1 - layer.pixels[i].r / 255.0) * 255);
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
                add_channel(trackingImage, value, 'r');
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "addgreen") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int value = std::stoi(argv[++argIndex]);
                add_channel(trackingImage, value, 'g');
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "addblue") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int value = std::stoi(argv[++argIndex]);
                add_channel(trackingImage, value, 'b');
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
                scale_channel(trackingImage, factor, 'b');
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
                scale_channel(trackingImage, factor, 'g');
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
                scale_channel(trackingImage, factor, 'r');
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if (method == "overlay") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string layerFile = argv[++argIndex];
            Image layer;
            if (!layer.load(layerFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            overlay(trackingImage, layer);
        }
        else if (method == "screen") {
            if (argIndex + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string layerFile = argv[++argIndex];
            Image layer;
            if (!layer.load(layerFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            screen(trackingImage, layer);
        }
        else if (method == "onlyred") {
            only_red(trackingImage);
        }
        else if (method == "onlygreen") {
            only_green(trackingImage);
        }
        else if (method == "combine") {
            if (argIndex + 3 >= argc) {
                std::cerr << "Missing argument for combine method." << std::endl;
                return 1;
            }
            std::string rFile = argv[++argIndex];
            std::string gFile = argv[++argIndex];
            std::string bFile = argv[++argIndex];
            
            Image r, g, b;
            if (!r.load(rFile) || !g.load(gFile) || !b.load(bFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            combine(trackingImage, r, g, b);
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
