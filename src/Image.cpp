#include "Image.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Constructor to load an image
Image::Image(const std::string& filename) {
    load(filename);
}

// Load a TGA image file
bool Image::load(const std::string& filename) {
    // Open file in binary mode
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    // Reading file header and pixel data goes here...
    // Assume 24-bit uncompressed TGA (simplified)
    // Populate `width`, `height`, and `pixels` accordingly
    file.close();
    return true;
}

// Save a TGA image file
bool Image::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save file: " << filename << std::endl;
        return false;
    }
    // Write TGA header and pixel data...
    file.close();
    return true;
}

// Multiply pixels
void Image::multiply(const Image& other) {
    for (size_t i = 0; i < pixels.size(); i++) {
        pixels[i].r = static_cast<unsigned char>((pixels[i].r / 255.0f) * (other.pixels[i].r / 255.0f) * 255.0f);
        pixels[i].g = static_cast<unsigned char>((pixels[i].g / 255.0f) * (other.pixels[i].g / 255.0f) * 255.0f);
        pixels[i].b = static_cast<unsigned char>((pixels[i].b / 255.0f) * (other.pixels[i].b / 255.0f) * 255.0f);
    }
}

// Subtract pixels
void Image::subtract(const Image& other) {
    for (size_t i = 0; i < pixels.size(); i++) {
        pixels[i].r = std::max(0, pixels[i].r - other.pixels[i].r);
        pixels[i].g = std::max(0, pixels[i].g - other.pixels[i].g);
        pixels[i].b = std::max(0, pixels[i].b - other.pixels[i].b);
    }
}

// Flip image
void Image::flip() {
    std::reverse(pixels.begin(), pixels.end());
}

// Red-only filter
void Image::onlyRed() {
    for (auto& pixel : pixels) {
        pixel.g = 0;
        pixel.b = 0;
    }
}

// Green-only filter
void Image::onlyGreen() {
    for (auto& pixel : pixels) {
        pixel.r = 0;
        pixel.b = 0;
    }
}

// Blue-only filter
void Image::onlyBlue() {
    for (auto& pixel : pixels) {
        pixel.r = 0;
        pixel.g = 0;
    }
}

// Add value to red channel with clamping
void Image::addRed(int value) {
    for (auto& pixel : pixels) {
        int newVal = pixel.r + value;
        clampChannel(newVal);
        pixel.r = static_cast<unsigned char>(newVal);
    }
}

// Helper function for clamping channel values
void Image::clampChannel(int& channelValue) {
    channelValue = std::min(255, std::max(0, channelValue));
}
