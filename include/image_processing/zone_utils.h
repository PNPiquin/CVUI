#ifndef ZONE_UTILS_H
#define ZONE_UTILS_H

#include <cstdint>
#include <memory>
#include <vector>

#include "utils/matrix.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_borders(std::shared_ptr<Matrix<uint8_t>> img,
                                             uint8_t threshold = 0,
                                             int neighborhood_size = 2);
bool is_border(std::shared_ptr<Matrix<uint8_t>> img, int row, int col, uint8_t threshold, int neighborhood_size = 2);

std::vector<std::shared_ptr<Matrix<uint32_t>>> extract_zone_images(std::shared_ptr<Matrix<uint32_t>> rgba_img,
                                                                   std::shared_ptr<Matrix<uint8_t>> zone_img,
                                                                   int number_of_clusters);
}

#endif