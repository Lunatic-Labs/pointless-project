#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <errno.h>
#include <cstring>
#include <stdarg.h>

#include "./include/graphics.h"

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