#include <cassert>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <errno.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>

#include "./include/graphics.h"

std::string graphics_pixel_to_hex(Pixel p)
{
  char hex[8];
  std::sprintf(hex, "#%02X%02X%02X", p.red, p.green, p.blue);
  return std::string(hex);
}

Svg graphics_gen_svg_from_image(Image &img, float pixel_size, std::optional<std::string> outline)
{
  Svg svg(img.width*pixel_size, img.height*pixel_size);

  for (size_t i = 0; i < img.height; i++) {
    for (size_t j = 0; j < img.width; ++j) {
      Pixel &p = img(i, j);
      float x = j * pixel_size;
      float y = i * pixel_size;
      std::string hex = graphics_pixel_to_hex(p);
      std::string classname = std::to_string(i) + "." +  std::to_string(j);
      Svg::Rect rect(x, y, pixel_size, pixel_size, hex, outline, p.alpha, classname);
      // Svg::Rect rect(x, y, pixel_size, pixel_size, hex, outline, {}, classname);
      svg.add_shape(rect);
    }
  }

  return svg;
}

void graphics_create_ppm(Image &img, const char *filepath)
{
  FILE *fp = std::fopen(filepath, "wb");
  if (!fp) {
    std::fprintf(stderr, "Error: could not open file %s. Reason: %s\n",
                 filepath, std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  auto write_bytes = [&](const char *formatstr, ...) -> void {
    va_list args;
    va_start(args, formatstr);
    if (std::vfprintf(fp, formatstr, args) < 0) {
      std::fprintf(stderr, "Error: failed to write bytes to file %s. Reason: %s\n",
                   filepath, std::strerror(errno));
      std::exit(EXIT_FAILURE);
    }
    va_end(args);
  };

  write_bytes("P6 %d %d 255\n", img.height, img.width);

  for (size_t i = 0; i < img.height; ++i) {
    for (size_t j = 0; j < img.width; ++j) {
      Pixel &p = img(i, j);
      write_bytes("%c%c%c", p.red, p.green, p.blue);
    }
  }

  std::fclose(fp);
}

Image graphics_scale_ppm(Image &img, size_t scale)
{
  assert(scale != 0);
  Image scaled_img = Image{img.height * scale, img.width * scale};

  for (size_t i = 0; i < img.height; i++) {
    for (size_t j = 0; j < img.width; j++) {
      Pixel &color = img(i, j);
      for (size_t k = 0; k < scale; k++) {
        for (size_t l = 0; l < scale; l++) {
          scaled_img(i * scale + k, j * scale + l) = color;
        }
      }
    }
  }
  return scaled_img;
}

#define QUOTEF(x) ("\"" + std::to_string(x) + "\"")
#define QUOTES(s) ("\"" + s + "\"")

std::string Svg::Rect::make() const
{
  std::string s = stroke.has_value() ? "stroke=\"" + stroke.value() + "\"" : "";
  std::string o = opacity.has_value() ? " opacity=\"" + std::to_string(opacity.value()) + "\"" : "";
  std::string classname = html_classname.has_value() ? " class=\"" + html_classname.value() + "\"" : "";
  return "<rect x=" + QUOTEF(x) +
          " y=" + QUOTEF(y) +
          " width=" + QUOTEF(width) +
          " height=" + QUOTEF(height) +
          s +
          o +
          classname +
          " fill=" + QUOTES(fill) + "  />";
}

std::string Svg::Circle::make() const
{
  std::string s = stroke.has_value() ? " stroke=\"" + stroke.value() + "\"" : "";
  std::string o = opacity.has_value() ? " opacity=\"" + std::to_string(opacity.value()) + "\"" : "";
  std::string classname = html_classname.has_value() ? " class=\"" + html_classname.value() + "\"" : "";
  return "<circle cx=" + QUOTEF(x) +
          " cy=" + QUOTEF(y) +
          " r=" + QUOTEF(radius) +
          s +
          o +
          classname +
          " fill=" + QUOTES(fill) + "  />";
}

std::string Svg::build(bool border) {
  std::string header = "<svg width=" + QUOTEF(width) +
                        "height=" + QUOTEF(height) +
                        "xmlns=" + QUOTES(xmlns) +
                        "version=" + QUOTES(version)
                        + ">\n";
  std::string body = "";
  for (auto &line : lines) {
    body += line + "\n";
  }
  if (border) {
    body += "<rect x=\"0\" y=\"0\" width='100%' height='100%' stroke='black' stroke-width='5px' fill='none' />";
  }
  body += "</svg>\n";
  return header + body;
}
