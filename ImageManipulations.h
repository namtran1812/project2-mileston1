#ifndef IMAGEMANIPULATIONS_H
#define IMAGEMANIPULATIONS_H

#include "TGAImage.h"

void multiply(TGAImage& top, const TGAImage& bottom);
void screen(TGAImage& top, const TGAImage& bottom);
void subtract(TGAImage& top, const TGAImage& bottom);
void overlay(TGAImage& top, const TGAImage& bottom);
void addChannel(TGAImage& image, int channel, int value);
void scaleChannel(TGAImage& image, int channel, float scale);

#endif
