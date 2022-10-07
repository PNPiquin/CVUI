#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <cstdint>
#include <memory>

#include "utils/matrix.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_random_framing(std::shared_ptr<Matrix<uint32_t>> img, int n_tiles = 50);
}

#endif