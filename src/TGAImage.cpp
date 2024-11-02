#include "TGAImage.h"

bool TGAImage::readFromFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
    pixels.resize(header.width * header.height);

    for (Pixel &pixel : pixels) {
        file.read(reinterpret_cast<char*>(&pixel.b), 1);
        file.read(reinterpret_cast<char*>(&pixel.g), 1);
        file.read(reinterpret_cast<char*>(&pixel.r), 1);
    }
    file.close();
    return true;
}

bool TGAImage::writeToFile(const std::string &filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(&header), sizeof(Header));

    for (const Pixel &pixel : pixels) {
        file.write(reinterpret_cast<const char*>(&pixel.b), 1);
        file.write(reinterpret_cast<const char*>(&pixel.g), 1);
        file.write(reinterpret_cast<const char*>(&pixel.r), 1);
    }
    file.close();
    return true;
}

void TGAImage::multiply(const TGAImage &other) {
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].r = static_cast<unsigned char>(std::min(255, (pixels[i].r * other.pixels[i].r) / 255));
        pixels[i].g = static_cast<unsigned char>(std::min(255, (pixels[i].g * other.pixels[i].g) / 255));
        pixels[i].b = static_cast<unsigned char>(std::min(255, (pixels[i].b * other.pixels[i].b) / 255));
    }
}

void TGAImage::subtract(const TGAImage &other) {
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].r = static_cast<unsigned char>(std::max(0, pixels[i].r - other.pixels[i].r));
        pixels[i].g = static_cast<unsigned char>(std::max(0, pixels[i].g - other.pixels[i].g));
        pixels[i].b = static_cast<unsigned char>(std::max(0, pixels[i].b - other.pixels[i].b));
    }
}

void TGAImage::screen(const TGAImage &other) {
    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].r = static_cast<unsigned char>(255 - (255 - pixels[i].r) * (255 - other.pixels[i].r) / 255);
        pixels[i].g = static_cast<unsigned char>(255 - (255 - pixels[i].g) * (255 - other.pixels[i].g) / 255);
        pixels[i].b = static_cast<unsigned char>(255 - (255 - pixels[i].b) * (255 - other.pixels[i].b) / 255);
    }
}

void TGAImage::overlay(const TGAImage &other) {
    for (size_t i = 0; i < pixels.size(); ++i) {
        if (other.pixels[i].r < 128)
            pixels[i].r = static_cast<unsigned char>((pixels[i].r * other.pixels[i].r * 2) / 255);
        else
            pixels[i].r = static_cast<unsigned char>(255 - 2 * (255 - pixels[i].r) * (255 - other.pixels[i].r) / 255);

        if (other.pixels[i].g < 128)
            pixels[i].g = static_cast<unsigned char>((pixels[i].g * other.pixels[i].g * 2) / 255);
        else
            pixels[i].g = static_cast<unsigned char>(255 - 2 * (255 - pixels[i].g) * (255 - other.pixels[i].g) / 255);

        if (other.pixels[i].b < 128)
            pixels[i].b = static_cast<unsigned char>((pixels[i].b * other.pixels[i].b * 2) / 255);
        else
            pixels[i].b = static_cast<unsigned char>(255 - 2 * (255 - pixels[i].b) * (255 - other.pixels[i].b) / 255);
    }
}

void TGAImage::addColor(int red, int green, int blue) {
    for (Pixel &pixel : pixels) {
        pixel.r = static_cast<unsigned char>(std::clamp(static_cast<int>(pixel.r) + red, 0, 255));
        pixel.g = static_cast<unsigned char>(std::clamp(static_cast<int>(pixel.g) + green, 0, 255));
        pixel.b = static_cast<unsigned char>(std::clamp(static_cast<int>(pixel.b) + blue, 0, 255));
    }
}

void TGAImage::scaleColor(int red, int green, int blue) {
    for (Pixel &pixel : pixels) {
        pixel.r = static_cast<unsigned char>(std::clamp(pixel.r * red, 0, 255));
        pixel.g = static_cast<unsigned char>(std::clamp(pixel.g * green, 0, 255));
        pixel.b = static_cast<unsigned char>(std::clamp(pixel.b * blue, 0, 255));
    }
}

void TGAImage::extractChannel(const std::string &channel, TGAImage &output) const {
    output.header = header;
    output.pixels = pixels;

    for (Pixel &pixel : output.pixels) {
        if (channel == "red") {
            pixel.g = 0;
            pixel.b = 0;
        } else if (channel == "green") {
            pixel.r = 0;
            pixel.b = 0;
        } else if (channel == "blue") {
            pixel.r = 0;
            pixel.g = 0;
        }
    }
}

void TGAImage::combineChannels(const TGAImage &red, const TGAImage &green, const TGAImage &blue) {
    header = red.header;
    pixels = red.pixels;

    for (size_t i = 0; i < pixels.size(); ++i) {
        pixels[i].r = red.pixels[i].r;
        pixels[i].g = green.pixels[i].g;
        pixels[i].b = blue.pixels[i].b;
    }
}

void TGAImage::flip() {
    int width = header.width;
    int height = header.height;

    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width; ++x) {
            std::swap(pixels[y * width + x], pixels[(height - 1 - y) * width + x]);
        }
    }
}
