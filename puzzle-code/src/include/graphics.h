#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct Pixel {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  float alpha = 1.f;
};

// Returns `p` as "#RRGGBB", ignoring alpha.
std::string graphics_pixel_to_hex(Pixel p);

// A grid of pixels, `height` rows by `width` columns, all transparent at first.
struct Image {
  size_t width;
  size_t height;
  std::vector<Pixel> pixels;

  Image(size_t w, size_t h) : width(w), height(h), pixels(w*h, Pixel{0, 0, 0, 0}) {}

  // The pixel in row `i`, column `j`.
  Pixel &operator()(size_t i, size_t j) { return pixels[i*width + j]; }
  const Pixel &operator()(size_t i, size_t j) const { return pixels[i*width + j]; }
};

struct Svg {
  struct Shape {
    float x;
    float y;
    std::string fill;
    std::optional<std::string> stroke;
    std::optional<float> opacity;
    std::optional<std::string> html_classname;

    Shape(float _x, float _y, std::string _fill, std::optional<std::string> _stroke,
          std::optional<float> _opacity, std::optional<std::string> _html_classname)
      : x(_x), y(_y), fill(std::move(_fill)), stroke(std::move(_stroke)),
        opacity(_opacity), html_classname(std::move(_html_classname)) {}
    virtual ~Shape() = default;

    // Returns the shape as an SVG element.
    virtual std::string make() const = 0;

  protected:
    // Returns the stroke, opacity, class, and fill attributes that are set, each after a space.
    std::string common_attributes() const;
  };

  struct Rect : Shape {
    float width;
    float height;

    Rect(float _x, float _y, float _width, float _height, std::string _fill,
         std::optional<std::string> _stroke = {},
         std::optional<float> _opacity = {},
         std::optional<std::string> _html_classname = {})
      : Shape(_x, _y, std::move(_fill), std::move(_stroke), _opacity, std::move(_html_classname)),
        width(_width), height(_height) {}

    std::string make() const override;
  };

  struct Circle : Shape {
    float radius;

    Circle(float _x, float _y, float _radius, std::string _fill,
           std::optional<std::string> _stroke = {},
           std::optional<float> _opacity = {},
           std::optional<std::string> _html_classname = {})
      : Shape(_x, _y, std::move(_fill), std::move(_stroke), _opacity, std::move(_html_classname)),
        radius(_radius) {}

    std::string make() const override;
  };

  float width;
  float height;

  // The elements of the shapes added so far.
  std::vector<std::string> lines;

  Svg(float w, float h) : width(w), height(h) {}

  // Returns an <svg> element holding every shape, with a black border if `border`.
  std::string build(bool border) const;

  template <class S>
  void add_shape(const S &shape) { lines.push_back(shape.make()); }
};

// Returns an SVG with a `pixel_size` square for each pixel of `img` (using the pixel's alpha as its
// opacity), outlined in `outline` if it is given. Each square has the class "<row>.<column>".
Svg graphics_gen_svg_from_image(const Image &img, float pixel_size, std::optional<std::string> outline);

#endif // GRAPHICS_H
