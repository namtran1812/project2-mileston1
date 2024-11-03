#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "tgaimage.h"

using namespace std;

Pixel::Pixel() {
    blue = 0;
    green = 0;
    red = 0;
}

Pixel::Pixel(char b, char g, char r) {
    blue = b;
    green = g;
    red = r;
}

Picture::Picture() {
    idLength = 0;
    colorMapType = 0;
    dataTypeCode = 0;
    colorMapOrigin = 0;
    colorMapLength = 0;
    colorMapDepth = 0;
    xOrigin = 0;
    yOrigin = 0;
    width = 0;
    height = 0;
    bitsPerPixel = 0;
    imageDescriptor = 0;
}

Picture::Picture(char idL, char colorM, char dataT, short colorMapO, short colorMapL,
                 char colorMapD, short xOri, short yOri, short w, short h,
                 char bitsP, char imageD, vector<Pixel> pixel) {
    idLength = idL;
    colorMapType = colorM;
    dataTypeCode = dataT;
    colorMapOrigin = colorMapO;
    colorMapLength = colorMapL;
    colorMapDepth = colorMapD;
    xOrigin = xOri;
    yOrigin = yOri;
    width = w;
    height = h;
    bitsPerPixel = bitsP;
    imageDescriptor = imageD;
    pixels = pixel;
}

// Read and Write

void Picture::readData(const string& filePath, Picture& image) {
    ifstream file(filePath, ios::in | ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&image.idLength), sizeof(image.idLength));
        file.read(reinterpret_cast<char*>(&image.colorMapType), sizeof(image.colorMapType));
        file.read(reinterpret_cast<char*>(&image.dataTypeCode), sizeof(image.dataTypeCode));
        file.read(reinterpret_cast<char*>(&image.colorMapOrigin), sizeof(image.colorMapOrigin));
        file.read(reinterpret_cast<char*>(&image.colorMapLength), sizeof(image.colorMapLength));
        file.read(reinterpret_cast<char*>(&image.colorMapDepth), sizeof(image.colorMapDepth));
        file.read(reinterpret_cast<char*>(&image.xOrigin), sizeof(image.xOrigin));
        file.read(reinterpret_cast<char*>(&image.yOrigin), sizeof(image.yOrigin));
        file.read(reinterpret_cast<char*>(&image.width), sizeof(image.width));
        file.read(reinterpret_cast<char*>(&image.height), sizeof(image.height));
        file.read(reinterpret_cast<char*>(&image.bitsPerPixel), sizeof(image.bitsPerPixel));
        file.read(reinterpret_cast<char*>(&image.imageDescriptor), sizeof(image.imageDescriptor));

        int imageSize = image.width * image.height;
        image.pixels.resize(imageSize);

        for(unsigned int i = 0; i < imageSize; i++) {
            file.read(reinterpret_cast<char*>(&image.pixels[i].blue), sizeof(image.pixels[i].blue));
            file.read(reinterpret_cast<char*>(&image.pixels[i].green), sizeof(image.pixels[i].green));
            file.read(reinterpret_cast<char*>(&image.pixels[i].red), sizeof(image.pixels[i].red));
        }

        file.close();
    }
    else {
        cout << "File not found" << endl;
    }
}

void Picture::writeData(const string& filePath, Picture& image) {
    ofstream file(filePath, ios::out | ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(&image.idLength), sizeof(image.idLength));
        file.write(reinterpret_cast<char*>(&image.colorMapType), sizeof(image.colorMapType));
        file.write(reinterpret_cast<char*>(&image.dataTypeCode), sizeof(image.dataTypeCode));
        file.write(reinterpret_cast<char*>(&image.colorMapOrigin), sizeof(image.colorMapOrigin));
        file.write(reinterpret_cast<char*>(&image.colorMapLength), sizeof(image.colorMapLength));
        file.write(reinterpret_cast<char*>(&image.colorMapDepth), sizeof(image.colorMapDepth));
        file.write(reinterpret_cast<char*>(&image.xOrigin), sizeof(image.xOrigin));
        file.write(reinterpret_cast<char*>(&image.yOrigin), sizeof(image.yOrigin));
        file.write(reinterpret_cast<char*>(&image.width), sizeof(image.width));
        file.write(reinterpret_cast<char*>(&image.height), sizeof(image.height));
        file.write(reinterpret_cast<char*>(&image.bitsPerPixel), sizeof(image.bitsPerPixel));
        file.write(reinterpret_cast<char*>(&image.imageDescriptor), sizeof(image.imageDescriptor));

        int imageSize = image.width * image.height;
        image.pixels.resize(imageSize);

        for(unsigned int i = 0; i < imageSize; i++) {
            file.write(reinterpret_cast<char*>(&image.pixels[i].blue), sizeof(image.pixels[i].blue));
            file.write(reinterpret_cast<char*>(&image.pixels[i].green), sizeof(image.pixels[i].green));
            file.write(reinterpret_cast<char*>(&image.pixels[i].red), sizeof(image.pixels[i].red));
        }

        file.close();
    }
    else {
        cout << "File not found" << endl;
    }
}

void Picture::initializeImage(Picture& original, Picture& copy) {
    copy.idLength = original.idLength;
    copy.colorMapType = original.colorMapType;
    copy.dataTypeCode = original.dataTypeCode;
    copy.colorMapOrigin = original.colorMapOrigin;
    copy.colorMapLength = original.colorMapLength;
    copy.colorMapDepth = original.colorMapDepth;
    copy.xOrigin = original.xOrigin;
    copy.yOrigin = original.yOrigin;
    copy.width = original.width;
    copy.height = original.height;
    copy.bitsPerPixel = original.bitsPerPixel;
    copy.imageDescriptor = original.imageDescriptor;
    copy.pixels.resize(original.width * original.height);
}

// Algorithms and other functions

void Picture::multiply(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer) {
    for(unsigned int i = 0; i < outcomeLayer.pixels.size(); i++){
        float normalizedTopBlue = static_cast<float>(topLayer.pixels[i].blue) / 255.0f;
        float normalizedBotBlue = static_cast<float>(botLayer.pixels[i].blue) / 255.0f;
        float outcomeBlue = normalizedTopBlue * normalizedBotBlue * 255.0f;
        outcomeLayer.pixels[i].blue = (unsigned char)(outcomeBlue + 0.5f);

        float normalizedTopGreen = static_cast<float>(topLayer.pixels[i].green) / 255.0f;
        float normalizedBotGreen = static_cast<float>(botLayer.pixels[i].green) / 255.0f;
        float outcomeGreen = normalizedTopGreen * normalizedBotGreen * 255.0f;
        outcomeLayer.pixels[i].green = (unsigned char)(outcomeGreen + 0.5f);

        float normalizedTopRed = static_cast<float>(topLayer.pixels[i].red) / 255.0f;
        float normalizedBotRed = static_cast<float>(botLayer.pixels[i].red) / 255.0f;
        float outcomeRed = normalizedTopRed * normalizedBotRed * 255.0f;
        outcomeLayer.pixels[i].red = (unsigned char)(outcomeRed + 0.5f);
    }
}

void Picture::subtract(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer) {
    int temp;

    for (unsigned int i = 0; i < outcomeLayer.pixels.size(); i++) {
        temp = (int) topLayer.pixels[i].blue - (int) botLayer.pixels[i].blue;
        if (temp < 0) {
            temp = 0;
        }
        outcomeLayer.pixels[i].blue = (unsigned char) temp;

        temp = (int) topLayer.pixels[i].green - (int) botLayer.pixels[i].green;
        if (temp < 0) {
            temp = 0;
        }
        outcomeLayer.pixels[i].green = (unsigned char) temp;

        temp = (int) topLayer.pixels[i].red - (int) botLayer.pixels[i].red;
        if (temp < 0) {
            temp = 0;
        }
        outcomeLayer.pixels[i].red = (unsigned char) temp;
    }
}

void Picture::overlay(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer) {
    for(unsigned int i = 0; i < outcomeLayer.pixels.size(); i++){
        float normalizedTopBlue = (float)topLayer.pixels[i].blue / 255;
        float normalizedBotBlue = (float)botLayer.pixels[i].blue / 255;
        if(normalizedBotBlue <= 0.5) {
            float outcomeBlue = 2 * normalizedTopBlue * normalizedBotBlue * 255;
            outcomeLayer.pixels[i].blue = (unsigned char)(outcomeBlue + 0.5f);
        }
        else {
            float outcomeBlue = (1 - (2 * (1 - normalizedTopBlue) * (1 - normalizedBotBlue))) * 255;
            outcomeLayer.pixels[i].blue = (unsigned char)(outcomeBlue + 0.5f);
        }

        float normalizedTopGreen = (float)topLayer.pixels[i].green / 255;
        float normalizedBotGreen = (float)botLayer.pixels[i].green / 255;
        if(normalizedBotGreen <= 0.5) {
            float outcomeGreen = 2 * normalizedTopGreen * normalizedBotGreen * 255;
            outcomeLayer.pixels[i].green = (unsigned char)(outcomeGreen + 0.5f);
        }
        else {
            float outcomeGreen = (1 - (2 * (1 - normalizedTopGreen) * (1 - normalizedBotGreen))) * 255;
            outcomeLayer.pixels[i].green = (unsigned char)(outcomeGreen + 0.5f);
        }

        float normalizedTopRed = (float)topLayer.pixels[i].red / 255;
        float normalizedBotRed = (float)botLayer.pixels[i].red / 255;
        if(normalizedBotRed <= 0.5) {
            float outcomeRed = 2 * normalizedTopRed * normalizedBotRed * 255;
            outcomeLayer.pixels[i].red = (unsigned char)(outcomeRed + 0.5f);
        }
        else {
            float outcomeRed = (1 - (2 * (1 - normalizedTopRed) * (1 - normalizedBotRed))) * 255;
            outcomeLayer.pixels[i].red = (unsigned char)(outcomeRed + 0.5f);
        }

    }

}

void Picture::screen(Picture& topLayer, Picture& botLayer, Picture& outcomeLayer) {
    for (unsigned int i = 0; i < outcomeLayer.pixels.size(); i++) {
        float normalizedTopBlue = (float) topLayer.pixels[i].blue / 255;
        float normalizedBotBlue = (float) botLayer.pixels[i].blue / 255;
        float outcomeBlue = (1 - ((1 - normalizedTopBlue) * (1 - normalizedBotBlue))) * 255;
        outcomeLayer.pixels[i].blue = (unsigned char) (outcomeBlue + 0.5f);

        float normalizedTopGreen = (float) topLayer.pixels[i].green / 255;
        float normalizedBotGreen = (float) botLayer.pixels[i].green / 255;
        float outcomeGreen = (1 - ((1 - normalizedTopGreen) * (1 - normalizedBotGreen))) * 255;
        outcomeLayer.pixels[i].green = (unsigned char) (outcomeGreen + 0.5f);

        float normalizedTopRed = (float) topLayer.pixels[i].red / 255;
        float normalizedBotRed = (float) botLayer.pixels[i].red / 255;
        float outcomeRed = (1 - ((1 - normalizedTopRed) * (1 - normalizedBotRed))) * 255;
        outcomeLayer.pixels[i].red = (unsigned char) (outcomeRed + 0.5f);
    }

}


void Picture::combine(Picture& redLayer, Picture& greenLayer, Picture& blueLayer, Picture& outcomeLayer) {
    for (unsigned int i = 0; i < redLayer.pixels.size(); i++) {
        outcomeLayer.pixels[i].red = redLayer.pixels[i].red;
        outcomeLayer.pixels[i].green = greenLayer.pixels[i].green;
        outcomeLayer.pixels[i].blue = blueLayer.pixels[i].blue;
    }
}

void Picture::flip(Picture& layer, Picture& outcomeLayer) {
    reverse(layer.pixels.begin(), layer.pixels.end());
}

void Picture::onlyred(Picture& layer, Picture& outcomeLayer) {
    for (unsigned int i = 0; i < layer.pixels.size(); i++) {
        outcomeLayer.pixels[i].blue = layer.pixels[i].red;
        outcomeLayer.pixels[i].green = layer.pixels[i].red;
        outcomeLayer.pixels[i].red = layer.pixels[i].red;
    }
}

void Picture::onlygreen(Picture& layer, Picture& outcomeLayer) {
    for (unsigned int i = 0; i < layer.pixels.size(); i++) {
        outcomeLayer.pixels[i].blue = layer.pixels[i].green;
        outcomeLayer.pixels[i].green = layer.pixels[i].green;
        outcomeLayer.pixels[i].red = layer.pixels[i].green;
    }
}

void Picture::onlyblue(Picture& layer, Picture& outcomeLayer) {
    for (unsigned int i = 0; i < layer.pixels.size(); i++) {
        outcomeLayer.pixels[i].blue = layer.pixels[i].blue;
        outcomeLayer.pixels[i].green = layer.pixels[i].blue;
        outcomeLayer.pixels[i].red = layer.pixels[i].blue;
    }
}

void Picture::addred(Picture& layer, int value, Picture& outcomeLayer) {
    cout << "adding red" << endl;
    for(int i = 0; i < layer.pixels.size(); i++){
        int temp = int(layer.pixels[i].red) + value;
        if(temp > 255){
            outcomeLayer.pixels[i].red = (unsigned char)(255);
        }
        else if(temp < 0){
            outcomeLayer.pixels[i].red = (unsigned char)(0);
        }
        else {
            layer.pixels[i].red = (unsigned char)(temp);
        }
    }
}

void Picture::addgreen(Picture& layer, int value, Picture& outcomeLayer) {
    for(int i = 0; i < layer.pixels.size(); i++){
        int temp = int(layer.pixels[i].green) + value;
        if(temp > 255){
            outcomeLayer.pixels[i].green = (unsigned char)(255);
        }
        else if(temp < 0){
            outcomeLayer.pixels[i].green = (unsigned char)(0);
        }
        else {
            layer.pixels[i].green = (unsigned char)(temp);
        }
    }
}

void Picture::addblue(Picture& layer, int value, Picture& outcomeLayer) {
    for(int i = 0; i < layer.pixels.size(); i++){
        int temp = int(layer.pixels[i].blue) + value;
        if(temp > 255){
            outcomeLayer.pixels[i].blue = (unsigned char)(255);
        }
        else if(temp < 0){
            outcomeLayer.pixels[i].blue = (unsigned char)(0);
        }
        else {
            layer.pixels[i].blue = (unsigned char)(temp);
        }
    }
}

void Picture::scalered(Picture& layer, unsigned int value, Picture& outcomeLayer) {
    for (int i = 0; i < layer.pixels.size(); i++) {
        int temp = layer.pixels[i].red * value;
        if (temp > 255) {
            temp = 255;
        }
        outcomeLayer.pixels[i].red = temp;
    }
}

void Picture::scalegreen(Picture& layer, unsigned int value, Picture& outcomeLayer) {
    for (int i = 0; i < layer.pixels.size(); i++) {
        int temp = layer.pixels[i].green * value;
        if (temp > 255) {
            temp = 255;
        }
        outcomeLayer.pixels[i].green = temp;
    }
}

void Picture::scaleblue(Picture& layer, unsigned int value, Picture& outcomeLayer) {
    for (int i = 0; i < layer.pixels.size(); i++) {
        int temp = layer.pixels[i].blue * value;
        if (temp > 255) {
            temp = 255;
        }
        outcomeLayer.pixels[i].blue = temp;
    }
}
