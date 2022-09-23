#ifndef IMAGE_PROCESSING_SPATIAL_FILTERING_HPP
#define IMAGE_PROCESSING_SPATIAL_FILTERING_HPP

#include <cstdint>
#include <memory>

#include "utils/matrix.h"

namespace ip {
void apply_kernel(std::shared_ptr<Matrix<uint8_t>> input_img,
                  std::shared_ptr<Matrix<float>> kernel,
                  std::shared_ptr<Matrix<uint8_t>> output_img);
uint8_t apply_kernel_on_pixel(std::shared_ptr<Matrix<uint8_t>> input_img,
                              std::shared_ptr<Matrix<float>> kernel,
                              size_t row,
                              size_t col);
}

#endif