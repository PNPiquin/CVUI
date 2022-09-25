#include "image_processing/normalization.h"

namespace ip {
uint8_t normalize_value(uint8_t value, float min, float max)
{
  return uint8_t((value - min) * (255.f / (max - min)));
}

std::vector<uint8_t> find_min_max(std::shared_ptr<Matrix<uint8_t>> input_img)
{
  uint8_t min = 255;
  uint8_t max = 0;
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      uint8_t value = input_img->operator()(row, col);
      if (value > max) {
        max = value;
      }
      if (value < min) {
        min = value;
      }
    }
  }
  std::vector<uint8_t> min_max{ min, max };
  return min_max;
}

std::shared_ptr<Matrix<uint8_t>> min_max_normalization(std::shared_ptr<Matrix<uint8_t>> input_img)
{
  std::vector<uint8_t> min_max = find_min_max(input_img);
  float min = float(min_max[0]);
  float max = float(min_max[1]);
  std::shared_ptr<Matrix<uint8_t>> res_img =
    std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      res_img->operator()(row, col) = normalize_value(input_img->operator()(row, col), min, max);
    }
  }
  return res_img;
}

// RGBA
std::vector<uint8_t> find_min_max(std::shared_ptr<Matrix<uint32_t>> input_img)
{
  uint8_t r_min = 255, g_min = 255, b_min = 255;
  uint8_t r_max = 0, g_max = 0, b_max = 0;
  ;
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      uint32_t rgba_pixel = input_img->operator()(row, col);

      uint8_t _r = (rgba_pixel & 0xff000000) >> 24;
      uint8_t _g = (rgba_pixel & 0x00ff0000) >> 16;
      uint8_t _b = (rgba_pixel & 0x0000ff00) >> 8;

      if (_r > r_max) {
        r_max = _r;
      }
      if (_r < r_min) {
        r_min = _r;
      }
      if (_g > g_max) {
        g_max = _g;
      }
      if (_g < g_min) {
        g_min = _g;
      }
      if (_b > b_max) {
        b_max = _b;
      }
      if (_b < b_min) {
        b_min = _b;
      }
    }
  }
  std::vector<uint8_t> min_max{ r_min, r_max, g_min, g_max, b_min, b_max };
  return min_max;
}

std::shared_ptr<Matrix<uint32_t>> min_max_normalization(std::shared_ptr<Matrix<uint32_t>> input_img)
{
  std::vector<uint8_t> min_max = find_min_max(input_img);
  float r_min = float(min_max[0]), g_min = float(min_max[2]), b_min = float(min_max[4]);
  float r_max = float(min_max[1]), g_max = float(min_max[3]), b_max = float(min_max[5]);
  std::shared_ptr<Matrix<uint32_t>> res_img =
    std::make_shared<Matrix<uint32_t>>(input_img->get_rows(), input_img->get_cols());
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      uint32_t rgba_pixel = input_img->operator()(row, col);

      uint8_t _r = normalize_value((rgba_pixel & 0xff000000) >> 24, r_min, r_max);
      uint8_t _g = normalize_value((rgba_pixel & 0x00ff0000) >> 16, g_min, g_max);
      uint8_t _b = normalize_value((rgba_pixel & 0x0000ff00) >> 8, b_min, b_max);

      res_img->operator()(row, col) = uint32_t(_r << 24 | _g << 16 | _b << 8 | 255);
    }
  }
  return res_img;
}
}