#ifndef IMAGE_PROCESSING_MORPHOLOGICAL_OPERATIONS_H
#define IMAGE_PROCESSING_MORPHOLOGICAL_OPERATIONS_H

#include <cstdint>
#include <memory>

#include "utils/matrix.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> apply_h_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size = 2);
std::shared_ptr<Matrix<uint8_t>> apply_v_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size = 2);

std::shared_ptr<Matrix<uint32_t>> apply_h_gradient(std::shared_ptr<Matrix<uint32_t>> input_img,
                                                   size_t gradient_size = 2);
std::shared_ptr<Matrix<uint32_t>> apply_v_gradient(std::shared_ptr<Matrix<uint32_t>> input_img,
                                                   size_t gradient_size = 2);
}

#endif