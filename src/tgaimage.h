#ifndef tga_image_h
#define tga_imaga_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Pixel{
    public:
        unsigned char blue;
        unsigned char green;
        unsigned char red;

        Pixel();
        Pixel(char b, char g, char r);
};

class Picture{
    public:
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
        vector<Pixel> pixels;

        Picture();
        Picture(char idL, char colorM, char dataT, short colorMapO, short colorMapL,
                char colorMapD, short xOri, short yOri, short w, short h,
                char bitsP, char imageD, vector<Pixel> pixel);

        void readData(const string& filePath, Picture& image);
        void writeData(const string& filePath, Picture& image);
        void initializeImage(Picture& original, Picture& copy);
        void multiply(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer);
        void subtract(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer);
        void overlay(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer);
        void screen(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer);
        void combine(Picture& redLayer, Picture& greenLayer, Picture& blueLayer, Picture& outcomeLayer);
        void flip(Picture& layer, Picture& outcomeLayer);
        void onlyred(Picture& layer, Picture& outcomeLayer);
        void onlygreen(Picture& layer, Picture& outcomeLayer);
        void onlyblue(Picture& layer, Picture& outcomeLayer);
        void addred(Picture& layer, int value, Picture& outcomeLayer);
        void addgreen(Picture& layer, int value, Picture& outcomeLayer);
        void addblue(Picture& layer, int value, Picture& outcomeLayer);
        void scalered(Picture& layer, unsigned int value, Picture& outcomeLayer);
        void scalegreen(Picture& layer, unsigned int value, Picture& outcomeLayer);
        void scaleblue(Picture& layer, unsigned int value, Picture& outcomeLayer);
};

#endif //PROJECT2IMAGEPROCESSING_HEADER_H
