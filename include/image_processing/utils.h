#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H

#include <cstdint>
#include <memory>

#include <utils/matrix.h>

struct RGBAPixel
{
  uint8_t r, g, b, a;

  RGBAPixel()
  {
    r = 0;
    g = 0;
    b = 0;
    a = 255;
  }
  RGBAPixel(uint8_t _r, uint8_t _g, uint8_t _b)
  {
    r = _r;
    g = _g;
    b = _b;
    a = 255;
  }
  RGBAPixel(uint32_t rgba_value)
  {
    r = (rgba_value & 0xff000000) >> 24;
    g = (rgba_value & 0x00ff0000) >> 16;
    b = (rgba_value & 0x0000ff00) >> 8;
    a = rgba_value & 0x000000ff;
  }
  uint32_t to_uint32_t() { return uint32_t(r << 24 | g << 16 | b << 8 | a); }
};

namespace ip {
std::shared_ptr<Matrix<uint8_t>> rgba_to_gray(std::shared_ptr<Matrix<uint32_t>> rgba_img);
}

#endif