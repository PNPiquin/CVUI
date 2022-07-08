#include "image_processing/zone_utils.h"
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

std::vector<std::shared_ptr<Matrix<uint32_t>>> extract_zone_images(std::shared_ptr<Matrix<uint32_t>> rgba_img,
                                                                   std::shared_ptr<Matrix<uint8_t>> zone_img,
                                                                   int number_of_clusters)
{
  std::vector<std::shared_ptr<Matrix<uint32_t>>> extracted_images;

  std::vector<uint8_t> zone_values;
  uint8_t color_inc = uint8_t(255 / (number_of_clusters - 1));
  for (int index = 0; index < number_of_clusters; ++index) {
    zone_values.push_back(index * color_inc);
  }

  const size_t rows = rgba_img->get_rows();
  const size_t cols = rgba_img->get_cols();
  for (const auto& value : zone_values) {
    size_t row_min(rows), row_max(0), col_min(cols), col_max(0);
    // Get zone ROI
    for (size_t row = 0; row < rows; ++row) {
      for (size_t col = 0; col < cols; ++col) {
        if (zone_img->operator()(row, col) == value) {
          if (row < row_min) {
            row_min = row;
          }
          if (row > row_max) {
            row_max = row;
          }
          if (col < col_min) {
            col_min = col;
          }
          if (col > col_max) {
            col_max = col;
          }
        }
      }
    }

    // Create a sub-image with zone
    const size_t sub_rows = row_max - row_min;
    const size_t sub_cols = col_max - col_min;
    auto sub_img = std::make_shared<Matrix<uint32_t>>(sub_rows, sub_cols);
    for (size_t row = 0; row < sub_rows; ++row) {
      for (size_t col = 0; col < sub_cols; ++col) {
        if (zone_img->operator()(row + row_min, col + col_min) == value) {
          sub_img->operator()(row, col) = rgba_img->operator()(row + row_min, col + col_min);
        }
      }
    }
    extracted_images.push_back(sub_img);
  }
  return extracted_images;
}
}
