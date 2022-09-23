#include "image_processing/spatial_filtering.hpp"

namespace ip {
void apply_kernel(std::shared_ptr<Matrix<uint8_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<uint8_t>> output_img)
{
  if (input_img->get_rows() != output_img->get_rows() || input_img->get_cols() != output_img->get_cols()) {
    output_img = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());
  }

  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      output_img->operator()(row, col) = apply_kernel_on_pixel(input_img, kernel, row, col);
    }
  }
}

uint8_t apply_kernel_on_pixel(std::shared_ptr<Matrix<uint8_t>> input_img,
                              std::shared_ptr<Matrix<float>> kernel,
                              size_t row,
                              size_t col)
{
  int rows = input_img->get_rows();
  int cols = input_img->get_cols();
  int kernel_semi_rows = (kernel->get_rows() - 1) / 2;
  int kernel_semi_cols = (kernel->get_cols() - 1) / 2;
  uint8_t acc = 0;
  int acc_cpt = 0;

  for (size_t x = 0; x < kernel->get_rows(); ++x) {
    for (size_t y = 0; y < kernel->get_cols(); ++y) {
      // Check if indexes are in image
      int row_index = row + x - kernel_semi_rows;
      if (row_index < 0 || row_index >= rows) {
        continue;
      }
      int col_index = col + y - kernel_semi_cols;
      if (col_index < 0 || col_index >= cols) {
        continue;
      }
      ++acc_cpt;
      acc += uint8_t(input_img->operator()(row_index, col_index) * kernel->operator()(x, y));
    }
  }
  return acc;
}
}