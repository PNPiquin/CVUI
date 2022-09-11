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
}
