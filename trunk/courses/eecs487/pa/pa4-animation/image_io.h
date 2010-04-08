#ifndef __IMAGE_IO_H
#define __IMAGE_IO_H

#include "image.h"

class ImageIO {
public:

  enum ImageFormatEnum {
    FORMAT_NONE = -1,
    FORMAT_PNG = 0,
    FORMAT_JPEG = 1
  };

  static Image* LoadJPEG(const char* filename);
  static Image* LoadPNG(const char* filename);
  static ImageFormatEnum recognize_image_format(const char* filename);
};

#endif  // __IMAGE_IO_H

