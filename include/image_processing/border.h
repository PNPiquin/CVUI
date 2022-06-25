#ifndef BORDER_H
#define BORDER_H

#include <cstdint>
#include <memory>

#include "utils/matrix.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_borders(std::shared_ptr<Matrix<uint8_t>> img,
                                             uint8_t threshold = 0,
                                             int neighborhood_size = 2);
bool is_border(std::shared_ptr<Matrix<uint8_t>> img, int row, int col, uint8_t threshold, int neighborhood_size = 2);
}

#endif