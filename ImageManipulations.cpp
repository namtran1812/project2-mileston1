#include "ImageManipulations.h"
#include <algorithm>

void multiply(TGAImage& top, const TGAImage& bottom) {
    for (size_t i = 0; i < top.pixels.size(); ++i) {
        top.pixels[i].red = std::min((top.pixels[i].red * bottom.pixels[i].red) / 255, 255);
        top.pixels[i].green = std::min((top.pixels[i].green * bottom.pixels[i].green) / 255, 255);
        top.pixels[i].blue = std::min((top.pixels[i].blue * bottom.pixels[i].blue) / 255, 255);
    }
}

void screen(TGAImage& top, const TGAImage& bottom) {
    for (size_t i = 0; i < top.pixels.size(); ++i) {
        top.pixels[i].red = 255 - ((255 - top.pixels[i].red) * (255 - bottom.pixels[i].red) / 255);
        top.pixels[i].green = 255 - ((255 - top.pixels[i].green) * (255 - bottom.pixels[i].green) / 255);
        top.pixels[i].blue = 255 - ((255 - top.pixels[i].blue) * (255 - bottom.pixels[i].blue) / 255);
    }
}

void subtract(TGAImage& top, const TGAImage& bottom) {
    for (size_t i = 0; i < top.pixels.size(); ++i) {
        top.pixels[i].red = std::max(top.pixels[i].red - bottom.pixels[i].red, 0);
        top.pixels[i].green = std::max(top.pixels[i].green - bottom.pixels[i].green, 0);
        top.pixels[i].blue = std::max(top.pixels[i].blue - bottom.pixels[i].blue, 0);
    }
}

void overlay(TGAImage& top, const TGAImage& bottom) {
    for (size_t i = 0; i < top.pixels.size(); ++i) {
        top.pixels[i].red = (bottom.pixels[i].red <= 128) ? 
            (2 * top.pixels[i].red * bottom.pixels[i].red / 255) : 
            (255 - 2 * (255 - top.pixels[i].red) * (255 - bottom.pixels[i].red) / 255);
        top.pixels[i].green = (bottom.pixels[i].green <= 128) ? 
            (2 * top.pixels[i].green * bottom.pixels[i].green / 255) : 
            (255 - 2 * (255 - top.pixels[i].green) * (255 - bottom.pixels[i].green) / 255);
        top.pixels[i].blue = (bottom.pixels[i].blue <= 128) ? 
            (2 * top.pixels[i].blue * bottom.pixels[i].blue / 255) : 
            (255 - 2 * (255 - top.pixels[i].blue) * (255 - bottom.pixels[i].blue) / 255);
    }
}

void addChannel(TGAImage& image, int channel, int value) {
    for (auto& pixel : image.pixels) {
        if (channel == 0) pixel.red = std::clamp(pixel.red + value, 0, 255);
        else if (channel == 1) pixel.green = std::clamp(pixel.green + value, 0, 255);
        else if (channel == 2) pixel.blue = std::clamp(pixel.blue + value, 0, 255);
    }
}

void scaleChannel(TGAImage& image, int channel, float scale) {
    for (auto& pixel : image.pixels) {
        if (channel == 0) pixel.red = std::clamp(static_cast<int>(pixel.red * scale), 0, 255);
        else if (channel == 1) pixel.green = std::clamp(static_cast<int>(pixel.green * scale), 0, 255);
        else if (channel == 2) pixel.blue = std::clamp(static_cast<int>(pixel.blue * scale), 0, 255);
    }
}
