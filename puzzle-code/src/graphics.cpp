#include <cstdio>

#include "./include/graphics.h"

// Formats a number for an SVG attribute without trailing zeros: 20, not 20.000000.
static std::string number(float value)
{
  char buf[32];
  std::snprintf(buf, sizeof buf, "%g", value);
  return buf;
}

// Returns ` name="value"`.
static std::string attribute(const std::string &name, const std::string &value)
{
  return " " + name + "=\"" + value + "\"";
}

std::string graphics_pixel_to_hex(Pixel p)
{
  char hex[16];
  std::snprintf(hex, sizeof hex, "#%02X%02X%02X", p.red, p.green, p.blue);
  return hex;
}

std::string Svg::Shape::common_attributes() const
{
  std::string attributes;
  if (stroke) {
    attributes += attribute("stroke", *stroke);
  }
  if (opacity) {
    attributes += attribute("opacity", number(*opacity));
  }
  if (html_classname) {
    attributes += attribute("class", *html_classname);
  }
  return attributes + attribute("fill", fill);
}

std::string Svg::Rect::make() const
{
  return "<rect"
    + attribute("x", number(x))
    + attribute("y", number(y))
    + attribute("width", number(width))
    + attribute("height", number(height))
    + common_attributes()
    + " />";
}

std::string Svg::Circle::make() const
{
  return "<circle"
    + attribute("cx", number(x))
    + attribute("cy", number(y))
    + attribute("r", number(radius))
    + common_attributes()
    + " />";
}

std::string Svg::build(bool border) const
{
  std::string svg = "<svg"
    + attribute("width", number(width))
    + attribute("height", number(height))
    + attribute("xmlns", "http://www.w3.org/2000/svg")
    + attribute("version", "1.1")
    + ">\n";
  for (const std::string &line : lines) {
    svg += line + "\n";
  }
  if (border) {
    svg += "<rect x=\"0\" y=\"0\" width=\"100%\" height=\"100%\" stroke=\"black\" stroke-width=\"5px\" fill=\"none\" />\n";
  }
  return svg + "</svg>\n";
}

Svg graphics_gen_svg_from_image(const Image &img, float pixel_size, std::optional<std::string> outline)
{
  Svg svg(img.width * pixel_size, img.height * pixel_size);

  for (size_t i = 0; i < img.height; i++) {
    for (size_t j = 0; j < img.width; j++) {
      const Pixel &p = img(i, j);
      Svg::Rect rect(j * pixel_size, i * pixel_size, pixel_size, pixel_size, graphics_pixel_to_hex(p),
                     outline, p.alpha, std::to_string(i) + "." + std::to_string(j));
      svg.add_shape(rect);
    }
  }

  return svg;
}
