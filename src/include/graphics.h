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

// `Image` is a simple DA that represents a 2D array of pixels.
struct Image {
  size_t width;
  size_t height;
  std::vector<Pixel> pixels;

  // Default constructor. Initializes an empty image.
  inline Image(size_t w, size_t h) : width(w), height(h), pixels(w*h) {}

  // Accessor to the pixel at position (i, j).
  // We need this because the pixels are stored in a 1D array
  // but being represented as a 2D array.
  inline Pixel &operator()(size_t i, size_t j)
  {
    return pixels[i*width+j];
  }
};

std::string graphics_gen_svg(Image &img, float pixel_size);

// Creates a PPM image from `im` and saves it to `filepath`.
void graphics_create_ppm(Image &img, const char *filepath);

// Scales the image by `scale` times.
// WARNING: This function is extremely slow.
Image graphics_scale_ppm(Image &img, size_t scale);

class Svg {
public:
  struct Shape {
    float x;
    float y;
    std::string fill;
    std::string stroke;

    virtual std::string make() const = 0;
  };

  struct Rec : Shape {
    float width;
    float height;
    Rec(float _x, float _y, std::string _fill, std::string _stroke, float _width, float _height)
      : width(_width), height(_height) {
      x = _x;
      y = _y;
      stroke = _stroke;
      width = _width;
    }

    std::string make() const override {
      return "";
    }
  };

  struct Circle : Shape {
    float radius;
    Circle(float _x, float _y, std::string _fill, std::string _stroke, float _radius)
      : radius(_radius) {
      x = _x;
      y = _y;
      stroke = _stroke;
      width = _width;
    }

    std::string make() const override {
      return "";
    }
  };

  std::string xmlns = "http://www.w3.org/2000/svg";
  std::string version = "1.1";

  float width;
  float height;
  std::vector<Shape> shapes;

  void add_shape(Shape shape) {
    (void)shape;
  }
};

#endif // GRAPHICS_H
