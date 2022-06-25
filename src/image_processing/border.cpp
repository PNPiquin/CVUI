#include "image_processing/border.h"
#include <cstdio>

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_borders(std::shared_ptr<Matrix<uint8_t>> img,
                                             uint8_t threshold,
                                             int neighborhood_size)
{
  std::shared_ptr<Matrix<uint8_t>> border_img = std::make_shared<Matrix<uint8_t>>(img->get_rows(), img->get_cols());

  // Here we consider a pixel is on a border if at least one of its neighbors has a different value
  for (size_t row = 0; row < img->get_rows(); ++row) {
    for (size_t col = 0; col < img->get_cols(); ++col) {
      if (is_border(img, row, col, threshold, neighborhood_size)) {
        border_img->operator()(row, col) = 255;
      }
    }
  }
  return border_img;
}

bool is_border(std::shared_ptr<Matrix<uint8_t>> img, int row, int col, uint8_t threshold, int neighborhood_size)
{
  uint8_t value = img->operator()(row, col);
  for (int row_shift = -neighborhood_size; row_shift <= neighborhood_size; ++row_shift) {
    for (int col_shift = -neighborhood_size; col_shift <= neighborhood_size; ++col_shift) {
      if (img->is_inside(row + row_shift, col + col_shift)) {
        if (img->operator()(row + row_shift, col + col_shift) > value + threshold ||
            img->operator()(row + row_shift, col + col_shift) < value - threshold) {
          return true;
        }
      }
    }
  }
  return false;
}
}
