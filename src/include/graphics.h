#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>

struct Pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct Image {
  size_t width;
  size_t height;
  std::vector<Pixel> pixels;
  
  inline Image(size_t width, size_t height)
    : width(width), height(height), pixels(width*height) {}
  
  inline Pixel &operator()(size_t i, size_t j)
  {
    return pixels[i*width+j];
  }
};

void graphics_create_ppm(Image &img, const char *filepath);


#endif // GRAPHICS_H