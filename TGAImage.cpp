#include "TGAImage.h"
#include <fstream>

bool TGAImage::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    
    // Skip 12 bytes, then read width and height
    file.seekg(12);
    file.read(reinterpret_cast<char*>(&width), 2);
    file.read(reinterpret_cast<char*>(&height), 2);

    // Allocate memory for pixels and read pixel data
    pixels.resize(width * height);
    file.seekg(18);
    file.read(reinterpret_cast<char*>(pixels.data()), width * height * sizeof(Pixel));

    file.close();
    return true;
}

bool TGAImage::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    // Header setup for a 24-bit uncompressed TGA file
    char header[18] = {0};
    header[2] = 2; // Uncompressed RGB
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 24; // 24 bits per pixel

    file.write(header, sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), width * height * sizeof(Pixel));
    file.close();
    return true;
}
