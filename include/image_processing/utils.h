#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H

#include <cmath>
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
  RGBAPixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
  {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
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

struct HSVAPixel
{
  uint8_t h, s, v, a;

  HSVAPixel()
  {
    h = 0;
    s = 0;
    v = 0;
    a = 255;
  }
  HSVAPixel(uint8_t _h, uint8_t _s, uint8_t _v)
  {
    h = _h;
    s = _s;
    v = _v;
    a = 255;
  }
  HSVAPixel(uint32_t hsva_value)
  {
    h = (hsva_value & 0xff000000) >> 24;
    s = (hsva_value & 0x00ff0000) >> 16;
    v = (hsva_value & 0x0000ff00) >> 8;
    a = hsva_value & 0x000000ff;
  }
  HSVAPixel(RGBAPixel rgba_pixel)
  {
    a = rgba_pixel.a;
    float r = rgba_pixel.r / 255.f, g = rgba_pixel.g / 255.f, b = rgba_pixel.b / 255.f;
    float c_max = std::max(std::max(r, g), b), c_min = std::min(std::min(r, g), b);
    float delta = c_max - c_min;

    s = c_max == 0.f ? 0.f : uint8_t((delta / c_max) * 255.f);
    v = uint8_t(delta * 255.f);

    // Compute hue
    if (delta == 0.f) {
      h = 0;
    } else if (c_max == r) {
      h = uint8_t(std::fmod((g - b) / delta, 6.f) * 255.f / 6.f);
    } else if (c_max == g) {
      h = uint8_t(((b - r) / delta + 2.f) * 255.f / 6.f);
    } else {
      h = uint8_t(((r - g) / delta + 4.f) * 255.f / 6.f);
    }
  }
  uint32_t to_uint32_t() { return uint32_t(h << 24 | s << 16 | v << 8 | a); }
  RGBAPixel to_rgba_pixel()
  {
    float _h = (float(h) / 255.f) * 360.f, _s = float(s) / 255.f, _v = float(v) / 255.f;
    float c = _v * _s;
    float x = c * (1 - std::abs(std::fmod(_h / 60.f, 2.f) - 1.f));
    float m = v - c;
    float _r, _g, _b;
    if (_h < 60.f) {
      _r = c, _g = x, _b = 0.f;
    } else if (_h < 120.f) {
      _r = x, _g = c, _b = 0.f;
    } else if (_h < 180.f) {
      _r = 0.f, _g = c, _b = x;
    } else if (_h < 240.f) {
      _r = 0.f, _g = x, _b = c;
    } else if (_h < 300.f) {
      _r = x, _g = 0.f, _b = c;
    } else {
      _r = c, _g = 0.f, _b = x;
    }
    return RGBAPixel(uint8_t((_r + m) * 255.f), uint8_t((_g + m) * 255.f), uint8_t((_b + m) * 255.f), a);
  }
};

namespace ip {
std::shared_ptr<Matrix<uint8_t>> rgba_to_gray(std::shared_ptr<Matrix<uint32_t>> rgba_img);
std::shared_ptr<Matrix<uint32_t>> gray_to_rgba(std::shared_ptr<Matrix<uint8_t>> gray_img);

// RGBA <=> HSVA
std::shared_ptr<Matrix<uint32_t>> rgba_to_hsva(std::shared_ptr<Matrix<uint32_t>> rgba_img);
std::shared_ptr<Matrix<uint32_t>> hsva_to_rgba(std::shared_ptr<Matrix<uint32_t>> hsva_img);
}

#endif