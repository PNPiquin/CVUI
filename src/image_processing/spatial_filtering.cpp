#include "image_processing/spatial_filtering.h"

#include <cmath>

namespace ip {
// GRAYSCALE
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
  float acc = 0.f;
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
      acc += input_img->operator()(row_index, col_index) * kernel->operator()(x, y);
    }
  }
  return uint8_t(std::sqrt(std::pow(acc, 2.f)));
}

// RGBA
void apply_kernel(std::shared_ptr<Matrix<uint32_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<uint32_t>> output_img)
{
  if (input_img->get_rows() != output_img->get_rows() || input_img->get_cols() != output_img->get_cols()) {
    output_img = std::make_shared<Matrix<uint32_t>>(input_img->get_rows(), input_img->get_cols());
  }

  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      output_img->operator()(row, col) = apply_kernel_on_pixel(input_img, kernel, row, col);
    }
  }
}

uint32_t apply_kernel_on_pixel(std::shared_ptr<Matrix<uint32_t>> input_img,
                               std::shared_ptr<Matrix<float>> kernel,
                               size_t row,
                               size_t col)
{
  int rows = input_img->get_rows();
  int cols = input_img->get_cols();
  int kernel_semi_rows = (kernel->get_rows() - 1) / 2;
  int kernel_semi_cols = (kernel->get_cols() - 1) / 2;
  float r_acc = 0.f;
  float g_acc = 0.f;
  float b_acc = 0.f;
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

      uint32_t rgba_pixel = input_img->operator()(row_index, col_index);
      r_acc += ((rgba_pixel & 0xff000000) >> 24) * kernel->operator()(x, y);
      g_acc += ((rgba_pixel & 0x00ff0000) >> 16) * kernel->operator()(x, y);
      b_acc += ((rgba_pixel & 0x0000ff00) >> 8) * kernel->operator()(x, y);
    }
  }
  r_acc = std::sqrt(std::pow(r_acc, 2.f));
  g_acc = std::sqrt(std::pow(g_acc, 2.f));
  b_acc = std::sqrt(std::pow(b_acc, 2.f));
  return uint32_t(uint8_t(r_acc) << 24 | uint8_t(g_acc) << 16 | uint8_t(b_acc) << 8 | 255);
}
}