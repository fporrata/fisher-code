#ifndef __IMAGE_H
#define __IMAGE_H

class Image {
public:
  Image(int width, int height, int channels);
  ~Image();
  unsigned char* get_pixels() const {
    return pixels_;
  }
  unsigned char* mutable_row(int row) {
    return pixels_ + channels_ * width_ * row;
  }
  int width() const {
    return width_;
  }
  int height() const {
    return height_;
  }
  int channels() const {
    return channels_;
  }
  bool hasAlpha() const {
    return channels_==4;
  }
private:
  int width_, height_;
  int channels_; // number of channels: 3 for RGB images, 4 for RGBA images
  unsigned char* pixels_;
};

#endif  // __IMAGE_H

