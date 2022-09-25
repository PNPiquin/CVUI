#ifndef IMAGE_PROCESSING_NORMALIZATION_H
#define IMAGE_PROCESSING_NORMALIZATION_H

#include <memory>
#include <vector>

#include "utils/matrix.h"

namespace ip {
uint8_t normalize_value(uint8_t value, float min, float max);

// GRAY
std::vector<uint8_t> find_min_max(std::shared_ptr<Matrix<uint8_t>> input_img);
std::shared_ptr<Matrix<uint8_t>> min_max_normalization(std::shared_ptr<Matrix<uint8_t>> input_img);

// RGBA
std::vector<uint8_t> find_min_max(std::shared_ptr<Matrix<uint32_t>> input_img);
std::shared_ptr<Matrix<uint32_t>> min_max_normalization(std::shared_ptr<Matrix<uint32_t>> input_img);
}

#endif