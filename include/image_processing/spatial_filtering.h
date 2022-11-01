#ifndef IMAGE_PROCESSING_SPATIAL_FILTERING_H
#define IMAGE_PROCESSING_SPATIAL_FILTERING_H

#include <cstdint>
#include <memory>

#include "utils/matrix.h"

namespace ip {
// Grayscale version
void apply_kernel(std::shared_ptr<Matrix<uint8_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<uint8_t>> output_img);
uint8_t apply_kernel_on_pixel(std::shared_ptr<Matrix<uint8_t>> input_img,
                              std::shared_ptr<Matrix<float>> kernel,
                              size_t row,
                              size_t col);

// RGB version
void apply_kernel(std::shared_ptr<Matrix<uint32_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<uint32_t>> output_img);
uint32_t apply_kernel_on_pixel(std::shared_ptr<Matrix<uint32_t>> input_img,
                               std::shared_ptr<Matrix<float>> kernel,
                               size_t row,
                               size_t col);

// Float version
void apply_kernel(std::shared_ptr<Matrix<uint8_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<float>> output_img);
float apply_kernel_on_pixel_float(std::shared_ptr<Matrix<uint8_t>> input_img,
                                  std::shared_ptr<Matrix<float>> kernel,
                                  size_t row,
                                  size_t col);

// Classic kernels
std::shared_ptr<Matrix<float>> create_gaussian_kernel(int kernel_size, float sigma);
std::shared_ptr<Matrix<float>> create_sobel_h_kernel();
std::shared_ptr<Matrix<float>> create_sobel_v_kernel();
}

#endif