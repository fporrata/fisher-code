// Simple image class for texture data storage.

#include "image.h"

#include <stdlib.h>

Image::Image(int width, int height, int channels) 
  : width_(width),
    height_(height),
    channels_(channels),
    pixels_(NULL)
{
  pixels_ = new unsigned char[width * height * channels];
}

Image::~Image() {
  delete[] pixels_;
}
