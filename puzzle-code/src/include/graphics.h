#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>
#include <optional>

struct Pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  float alpha = 1.f;
};

std::string graphics_pixel_to_hex(Pixel p);

// `Image` is a simple DS that represents a 2D array of pixels.
struct Image {
  size_t width;
  size_t height;
  std::vector<Pixel> pixels;

  // Default constructor. Initializes an empty image.
  inline Image(size_t w, size_t h) : width(w), height(h), pixels(w*h) {
    for (size_t i = 0; i < w*h; i++) {
      pixels[i] = {0,0,0,0};
    }
  }

  // Accessor to the pixel at position (i, j).
  // We need this because the pixels are stored in a 1D array
  // but being represented as a 2D array.
  inline Pixel &operator()(size_t i, size_t j) {
    return pixels[i*width+j];
  }
};

// Creates a PPM image from `im` and saves it to `filepath`.
void graphics_create_ppm(Image &img, const char *filepath);

// Scales the image by `scale` times.
// WARNING: This function is extremely slow.
Image graphics_scale_ppm(Image &img, size_t scale);

struct Svg {
  struct Shape {
    float x;
    float y;
    std::string fill;
    std::optional<std::string> stroke;
    std::optional<std::string> html_classname;
    std::optional<float> opacity;

    virtual std::string make() const = 0;
  };

  struct Rect : Shape {
    float width;
    float height;
    Rect(float _x, float _y,
        float _width, float _height,
        std::string _fill,
        std::optional<std::string> _stroke = {},
        std::optional<float> _opacity = {},
        std::optional<std::string> _html_classname = {})
      : width(_width), height(_height) {
      x = _x;
      y = _y;
      fill = _fill;
      stroke = _stroke;
      opacity = _opacity;
      html_classname = _html_classname;
      width = _width;
    }

    std::string make() const override;
  };

  struct Circle : Shape {
    float radius;
    Circle(float _x, float _y, float _radius, std::string _fill,
          std::optional<std::string> _stroke = {},
          std::optional<float> _opacity = {},
          std::optional<std::string> _html_classname = {})
      : radius(_radius) {
      x = _x;
      y = _y;
      fill = _fill;
      stroke = _stroke;
      opacity = _opacity;
      html_classname = _html_classname;
    }

    std::string make() const override;
  };

  // Required
  std::string xmlns = "http://www.w3.org/2000/svg";
  std::string version = "1.1";

  // width and height of the SVG itself.
  float width;
  float height;

  // The shapes that the SVG contains
  std::vector<std::string> lines;

  Svg(float w, float h) : width(w), height(h) {}

  std::string build(bool border);

  template <class Shape>
  inline void add_shape(Shape shape) {
    lines.push_back(shape.make());
  }
};

Svg graphics_gen_svg_from_image(Image &img, float pixel_size, std::optional<std::string> outline);

#endif // GRAPHICS_H
