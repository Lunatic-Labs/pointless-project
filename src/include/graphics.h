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
};

// `Image` is a simple DS that represents a 2D array of pixels.
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


#define QUOTEF(x) ("\"" + std::to_string(x) + "\"")
#define QUOTES(s) ("\"" + s + "\"")

struct Svg {
  struct Shape {
    float x;
    float y;
    std::string fill;
    std::optional<std::string> stroke;

    virtual std::string make() const = 0;
  };

  struct Rect : Shape {
    float width;
    float height;
    Rect(float _x, float _y, std::string _fill, std::optional<std::string> _stroke, float _width, float _height)
      : width(_width), height(_height) {
      x = _x;
      y = _y;
      fill = _fill;
      stroke = _stroke;
      width = _width;
    }

    std::string make() const override {
      std::string s = stroke ? "stroke=\"" + s + "\"" : "";
      return "<rect x=" + QUOTEF(x) +
             " y=" + QUOTEF(y) +
             " width=" + QUOTEF(width) +
             " height=" + QUOTEF(height) +
             s +
             " fill=" + QUOTES(fill) + "  />";
    }
  };

  struct Circle : Shape {
    float radius;
    Circle(float _x, float _y, std::string _fill, std::optional<std::string> _stroke, float _radius)
      : radius(_radius) {
      x = _x;
      y = _y;
      fill = _fill;
      stroke = _stroke;
      radius = _radius;
    }

    std::string make() const override {
      std::string s = stroke ? "stroke=\"" + s + "\"" : "";
      return "<rect cx=" + QUOTEF(x) +
             " cy=" + QUOTEF(y) +
             " r=" + QUOTEF(radius) +
             s +
             " fill=" + QUOTES(fill) + "  />";
    }
  };

  std::string xmlns = "http://www.w3.org/2000/svg";
  std::string version = "1.1";

  float width;
  float height;
  std::vector<std::string> lines;

  Svg(float w, float h) : width(w), height(h) {}

  std::string build(void) {
    std::string header = "<sv width=" + QUOTEF(width) +
                         "height=" + QUOTEF(height) +
                         "xmlns=" + QUOTES(xmlns) +
                         "version=" + QUOTES(version)
                         + "\n";
    std::string body = "";
    for (auto &line : lines) {
      body += line + "\n";
    }
    body += "</svg>\n";
    return header + body;
  }

  template <class Shape>
  void add_shape(Shape shape) {
    lines.push_back(shape.make());
  }
};

#endif // GRAPHICS_H
