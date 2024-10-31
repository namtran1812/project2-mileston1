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
    bool checkDimensions(const Image& other) const {
        return width == other.width && height == other.height;
    }
};

bool Image::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    unsigned char header[18];
    file.read(reinterpret_cast<char*>(header), sizeof(header));

    width = header[12] | (header[13] << 8);
    height = header[14] | (header[15] << 8);

    if (width <= 0 || height <= 0) {
        return false;
    }

    pixels.resize(width * height);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        return false;
    }

    file.close();
    return true;
}

bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    unsigned char header[18] = {0};
    header[2] = 2;  // Uncompressed true-color image
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24;  // 24 bits per pixel
    header[17] = 0x20;  // Image descriptor byte, sets origin in upper-left

    file.write(reinterpret_cast<const char*>(header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    if (!file) {
        return false;
    }

    file.close();
    return true;
}

Pixel multiply(const Pixel& p1, const Pixel& p2) {
    Pixel result;
    result.b = static_cast<unsigned char>(std::round((p1.b / 255.0) * (p2.b / 255.0) * 255));
    result.g = static_cast<unsigned char>(std::round((p1.g / 255.0) * (p2.g / 255.0) * 255));
    result.r = static_cast<unsigned char>(std::round((p1.r / 255.0) * (p2.r / 255.0) * 255));
    return result;
}

Pixel subtract(const Pixel& p1, const Pixel& p2) {
    return {
        static_cast<unsigned char>(std::max(0, p1.b - p2.b)),
        static_cast<unsigned char>(std::max(0, p1.g - p2.g)),
        static_cast<unsigned char>(std::max(0, p1.r - p2.r))
    };
}

void flipImage(Image& image) {
    std::vector<Pixel> flipped;
    flipped.reserve(image.pixels.size());
    for (int y = image.height - 1; y >= 0; --y) {
        for (int x = 0; x < image.width; ++x) {
            flipped.push_back(image.pixels[y * image.width + x]);
        }
    }
    image.pixels = std::move(flipped);
}

void add_channel(Image& image, int value, char channel) {
    for (Pixel& p : image.pixels) {
        int* target = nullptr;
        switch (channel) {
            case 'r': target = reinterpret_cast<int*>(&p.r); break;
            case 'g': target = reinterpret_cast<int*>(&p.g); break;
            case 'b': target = reinterpret_cast<int*>(&p.b); break;
        }
        if (target) {
            *target = std::min(255, std::max(0, static_cast<int>(*target) + value));
        }
    }
}

void scale_channel(Image& image, int factor, char channel) {
    for (Pixel& p : image.pixels) {
        unsigned char* target = nullptr;
        switch (channel) {
            case 'r': target = &p.r; break;
            case 'g': target = &p.g; break;
            case 'b': target = &p.b; break;
        }
        if (target) {
            *target = static_cast<unsigned char>(std::min(255, static_cast<int>(*target) * factor));
        }
    }
}

void overlay(Image& image, const Image& layer) {
    if (!image.checkDimensions(layer)) {
        std::cerr << "Error: Dimension mismatch among the input images for overlay operation." << std::endl;
        return;
    }
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i] = multiply(image.pixels[i], layer.pixels[i]);
    }
}

void screen(Image& image, const Image& layer) {
    if (!image.checkDimensions(layer)) {
        std::cerr << "Error: Dimension mismatch among the input images for screen operation." << std::endl;
        return;
    }
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].b = static_cast<unsigned char>(255 - ((255 - image.pixels[i].b) * (255 - layer.pixels[i].b) / 255.0));
        image.pixels[i].g = static_cast<unsigned char>(255 - ((255 - image.pixels[i].g) * (255 - layer.pixels[i].g) / 255.0));
        image.pixels[i].r = static_cast<unsigned char>(255 - ((255 - image.pixels[i].r) * (255 - layer.pixels[i].r) / 255.0));
    }
}

void only_red(Image& image) {
    for (Pixel& p : image.pixels) {
        p.g = p.b = 0;
    }
}

void only_green(Image& image) {
    for (Pixel& p : image.pixels) {
        p.r = p.b = 0;
    }
}

void combine(Image& image, const Image& r, const Image& g, const Image& b) {
    if (!image.checkDimensions(r) || !image.checkDimensions(g) || !image.checkDimensions(b)) {
        std::cerr << "Error: Dimension mismatch among the input images for combine operation." << std::endl;
        return;
    }
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        image.pixels[i].r = r.pixels[i].r;
        image.pixels[i].g = g.pixels[i].g;
        image.pixels[i].b = b.pixels[i].b;
    }
}

void printHelp() {
    std::cout << "Project 2: Image Processing, Fall 2024\n"
              << "\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]" << std::endl;
}

bool validateFilename(const std::string& filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".tga";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        printHelp();
        return 0;
    }

    if (!validateFilename(argv[1])) {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    if (argc < 3) {
        std::cerr << "Missing argument for input filename." << std::endl;
        return 1;
    }

    if (!validateFilename(argv[2])) {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    Image trackingImage;
    if (!trackingImage.load(argv[2])) {
        std::cerr << "File does not exist." << std::endl;
        return 1;
    }

    for (int i = 3; i < argc; ++i) {
        std::string method = argv[i];

        if (method == "multiply" || method == "subtract" || method == "overlay" || method == "screen") {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            std::string secondImageFile = argv[++i];
            if (!validateFilename(secondImageFile)) {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }
            Image secondImage;
            if (!secondImage.load(secondImageFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }
            if (!trackingImage.checkDimensions(secondImage)) {
                std::cerr << "Error: Dimension mismatch among the input images." << std::endl;
                return 1;
            }

            if (method == "multiply") {
                for (size_t j = 0; j < trackingImage.pixels.size(); ++j) {
                    trackingImage.pixels[j] = multiply(trackingImage.pixels[j], secondImage.pixels[j]);
                }
            } else if (method == "subtract") {
                for (size_t j = 0; j < trackingImage.pixels.size(); ++j) {
                    trackingImage.pixels[j] = subtract(trackingImage.pixels[j], secondImage.pixels[j]);
                }
            } else if (method == "overlay") {
                overlay(trackingImage, secondImage);
            } else if (method == "screen") {
                screen(trackingImage, secondImage);
            }
        } else if (method == "flip") {
            flipImage(trackingImage);
        } else if (method.find("add") == 0 || method.find("scale") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int value = std::stoi(argv[++i]);
                if (method == "addred") add_channel(trackingImage, value, 'r');
                else if (method == "addgreen") add_channel(trackingImage, value, 'g');
                else if (method == "addblue") add_channel(trackingImage, value, 'b');
                else if (method == "scalered") scale_channel(trackingImage, value, 'r');
                else if (method == "scalegreen") scale_channel(trackingImage, value, 'g');
                else if (method == "scaleblue") scale_channel(trackingImage, value, 'b');
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        } else if (method == "onlyred") {
            only_red(trackingImage);
        } else if (method == "onlygreen") {
            only_green(trackingImage);
        } else if (method == "combine") {
            if (i + 3 >= argc) {
                std::cerr << "Missing argument for combine method." << std::endl;
                return 1;
            }
            std::string rFile = argv[++i];
            std::string gFile = argv[++i];
            std::string bFile = argv[++i];

            if (!validateFilename(rFile) || !validateFilename(gFile) || !validateFilename(bFile)) {
                std::cerr << "Invalid argument, invalid file name." << std::endl;
                return 1;
            }

            Image r, g, b;
            if (!r.load(rFile) || !g.load(gFile) || !b.load(bFile)) {
                std::cerr << "Invalid argument, file does not exist." << std::endl;
                return 1;
            }

            if (!trackingImage.checkDimensions(r) || !trackingImage.checkDimensions(g) || !trackingImage.checkDimensions(b)) {
                std::cerr << "Error: Dimension mismatch among the input images for combine operation." << std::endl;
                return 1;
            }

            combine(trackingImage, r, g, b);
        } else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
    }

    if (!trackingImage.save(argv[1])) {
        std::cerr << "Error: Could not save file " << argv[1] << std::endl;
        return 1;
    }

    std::cout << "Output saved to " << argv[1] << std::endl;
    return 0;
}
