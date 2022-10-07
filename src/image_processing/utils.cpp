#include "image_processing/utils.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> rgba_to_gray(std::shared_ptr<Matrix<uint32_t>> rgba_img)
{
  std::shared_ptr<Matrix<uint8_t>> gray_img =
    std::make_shared<Matrix<uint8_t>>(rgba_img->get_rows(), rgba_img->get_cols());

  for (size_t i = 0; i < rgba_img->get_rows(); ++i) {
    for (size_t j = 0; j < rgba_img->get_cols(); ++j) {
      auto rgba_pixel = rgba_img->operator()(i, j);
      gray_img->operator()(i, j) =
        uint8_t(0.2989 * ((rgba_pixel & 0xff000000) >> 24) + 0.5870 * ((rgba_pixel & 0x00ff0000) >> 16) +
                0.1140 * ((rgba_pixel & 0x0000ff00) >> 8));
    }
  }
  return gray_img;
}
std::shared_ptr<Matrix<uint32_t>> gray_to_rgba(std::shared_ptr<Matrix<uint8_t>> gray_img)
{
  auto rgba_img = std::make_shared<Matrix<uint32_t>>(gray_img->get_rows(), gray_img->get_cols());
  for (size_t row = 0; row < gray_img->get_rows(); ++row) {
    for (size_t col = 0; col < gray_img->get_cols(); ++col) {
      uint8_t gray_value = gray_img->operator()(row, col);
      RGBAPixel pix(gray_value, gray_value, gray_value);
      rgba_img->operator()(row, col) = pix.to_uint32_t();
    }
  }
  return rgba_img;
}

std::shared_ptr<Matrix<uint32_t>> rgba_to_hsva(std::shared_ptr<Matrix<uint32_t>> rgba_img)
{
  auto hsva_img = std::make_shared<Matrix<uint32_t>>(rgba_img->get_rows(), rgba_img->get_cols());
  for (size_t row = 0; row < rgba_img->get_rows(); ++row) {
    for (size_t col = 0; col < rgba_img->get_cols(); ++col) {
      HSVAPixel pix(RGBAPixel(rgba_img->operator()(row, col)));
      hsva_img->operator()(row, col) = pix.to_uint32_t();
    }
  }
  return hsva_img;
}

std::shared_ptr<Matrix<uint32_t>> hsva_to_rgba(std::shared_ptr<Matrix<uint32_t>> hsva_img)
{
  auto rgba_img = std::make_shared<Matrix<uint32_t>>(hsva_img->get_rows(), hsva_img->get_cols());
  for (size_t row = 0; row < hsva_img->get_rows(); ++row) {
    for (size_t col = 0; col < hsva_img->get_cols(); ++col) {
      HSVAPixel pix(hsva_img->operator()(row, col));
      rgba_img->operator()(row, col) = pix.to_rgba_pixel().to_uint32_t();
    }
  }
  return rgba_img;
}
}
