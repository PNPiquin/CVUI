#ifndef IMAGE_PROCESSING_MORPHOLOGICAL_OPERATIONS_H
#define IMAGE_PROCESSING_MORPHOLOGICAL_OPERATIONS_H

#include <cstdint>
#include <memory>
#include <vector>

#include "utils/matrix.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> apply_h_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size = 2);
std::shared_ptr<Matrix<uint8_t>> apply_v_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size = 2);

std::shared_ptr<Matrix<uint32_t>> apply_h_gradient(std::shared_ptr<Matrix<uint32_t>> input_img,
                                                   size_t gradient_size = 2);
std::shared_ptr<Matrix<uint32_t>> apply_v_gradient(std::shared_ptr<Matrix<uint32_t>> input_img,
                                                   size_t gradient_size = 2);

template<typename T>
std::vector<T> extract_neighborhood(std::shared_ptr<Matrix<T>> input_img,
                                    std::shared_ptr<Matrix<uint8_t>> kernel,
                                    size_t row,
                                    size_t col)
{
  std::vector<T> res;

  size_t kernel_semi_height = kernel->get_rows() / 2;
  size_t kernel_semi_width = kernel->get_cols() / 2;
  size_t row_start = kernel_semi_height <= row ? row - kernel_semi_height : 0;
  size_t col_start = kernel_semi_width <= col ? col - kernel_semi_height : 0;
  size_t row_end = std::min(size_t(input_img->get_rows()), row_start + kernel->get_rows());
  size_t col_end = std::min(size_t(input_img->get_cols()), col_start + kernel->get_cols());

  for (size_t row_idx = row_start; row_idx < row_end; row_idx++) {
    for (size_t col_idx = col_start; col_idx < col_end; col_idx++) {
      if (kernel->operator()(row_idx - row_start, col_idx - col_start)) {
        res.push_back(input_img->operator()(row_idx, col_idx));
      }
    }
  }

  return res;
};

template<typename T>
T get_max(std::vector<T> pixels)
{
  if (pixels.size() == 0) {
    return T(0);
  }

  T res = pixels[0];
  for (size_t i = 1; i < pixels.size(); i++) {
    if (pixels[i] > res) {
      res = pixels[i];
    }
  }
  return res;
};

template<typename T>
T get_min(std::vector<T> pixels)
{
  if (pixels.size() == 0) {
    return T(0);
  }

  T res = pixels[0];
  for (size_t i = 1; i < pixels.size(); i++) {
    if (pixels[i] < res) {
      res = pixels[i];
    }
  }
  return res;
};
}

#endif