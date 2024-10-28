#ifndef TGAIMAGE_H
#define TGAIMAGE_H

#include <vector>
#include <string>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class TGAImage {
public:
    int width;
    int height;
    std::vector<Pixel> pixels;

    bool load(const std::string& filename);
    bool save(const std::string& filename);
};

#endif
