#ifndef IMAGE_PROCESSING_UTILS_H
#define IMAGE_PROCESSING_UTILS_H

#include <cmath>
#include <cstdint>
#include <memory>

#include <utils/matrix.h>

class RGBAPixel
{
public:
  uint8_t r, g, b, a;

  RGBAPixel();
  RGBAPixel(uint8_t _r, uint8_t _g, uint8_t _b);
  RGBAPixel(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);
  RGBAPixel(uint32_t rgba_value);
  uint32_t to_uint32_t();
};

class HSVAPixel
{
public:
  uint8_t h, s, v, a;

  HSVAPixel();
  HSVAPixel(uint8_t _h, uint8_t _s, uint8_t _v);
  HSVAPixel(uint32_t hsva_value);
  HSVAPixel(RGBAPixel rgba_pixel);
  uint32_t to_uint32_t();
  RGBAPixel to_rgba_pixel();

  void rotate_hue(double theta);
  void amplify_saturation(double factor);
  void amplify_value(double factor);
};

namespace ip {
std::shared_ptr<Matrix<uint8_t>> rgba_to_gray(std::shared_ptr<Matrix<uint32_t>> rgba_img);
std::shared_ptr<Matrix<uint32_t>> gray_to_rgba(std::shared_ptr<Matrix<uint8_t>> gray_img);

// RGBA <=> HSVA
std::shared_ptr<Matrix<uint32_t>> rgba_to_hsva(std::shared_ptr<Matrix<uint32_t>> rgba_img);
std::shared_ptr<Matrix<uint32_t>> hsva_to_rgba(std::shared_ptr<Matrix<uint32_t>> hsva_img);

// HSV
std::shared_ptr<Matrix<uint32_t>> hsva_transformation(std::shared_ptr<Matrix<uint32_t>> hsva_img,
                                                      double hue_rotation,
                                                      double saturation_factor,
                                                      double value_factor);
}

#endif