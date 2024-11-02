#ifndef TGAIMAGE_H
#define TGAIMAGE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

struct Pixel {
    unsigned char b, g, r; // TGA format uses BGR order
};

class TGAImage {
public:
    Header header;
    std::vector<Pixel> pixels;
    
    bool readFromFile(const std::string &filename);
    bool writeToFile(const std::string &filename) const;

    // Methods for image manipulation tasks
    void multiply(const TGAImage &other);
    void subtract(const TGAImage &other);
    void screen(const TGAImage &other);
    void overlay(const TGAImage &other);
    void addColor(int red, int green, int blue);
    void scaleColor(int red, int green, int blue);
    void extractChannel(const std::string &channel, TGAImage &output) const;
    void combineChannels(const TGAImage &red, const TGAImage &green, const TGAImage &blue);
    void flip();
};

#endif
