#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

class Image {
public:
    // Constructors
    Image() = default;
    Image(const std::string& filename);

    // Load and Save
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;

    // Image Manipulation Methods
    void multiply(const Image& other);
    void subtract(const Image& other);
    void overlay(const Image& other);
    void screen(const Image& other);
    void flip();
    void onlyRed();
    void onlyGreen();
    void onlyBlue();
    void addRed(int value);
    void addGreen(int value);
    void addBlue(int value);
    void scaleRed(int factor);
    void scaleGreen(int factor);
    void scaleBlue(int factor);

private:
    struct Pixel {
        unsigned char r, g, b;
    };

    int width;
    int height;
    std::vector<Pixel> pixels;

    void clampChannel(int& channelValue);
};

#endif
