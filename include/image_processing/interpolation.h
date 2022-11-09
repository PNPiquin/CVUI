#ifndef IMAGE_PROCESSING_INTERPOLATION_H
#define IMAGE_PROCESSING_INTERPOLATION_H

#include <cstdint>
#include <memory>

#include "image_processing/utils.h"
#include "utils/matrix.h"

namespace ip {
enum INTERPOLATION_TYPE
{
  BILINEAR
};

uint8_t interpolate_pixel_bilinear(uint8_t nw_pix,
                                   uint8_t ne_pix,
                                   uint8_t sw_pix,
                                   uint8_t se_pix,
                                   float row_fraction,
                                   float col_fraction);
uint8_t interpolate_pixel(std::shared_ptr<Matrix<uint8_t>> input_img,
                          float target_row,
                          float target_col,
                          INTERPOLATION_TYPE interp_type);
std::shared_ptr<Matrix<uint8_t>> resize(std::shared_ptr<Matrix<uint8_t>> input_img,
                                        size_t target_rows,
                                        size_t target_cols,
                                        INTERPOLATION_TYPE interp_type = INTERPOLATION_TYPE::BILINEAR);

RGBAPixel interpolate_pixel_bilinear(RGBAPixel nw_pix,
                                     RGBAPixel ne_pix,
                                     RGBAPixel sw_pix,
                                     RGBAPixel se_pix,
                                     float row_fraction,
                                     float col_fraction);
uint32_t interpolate_pixel(std::shared_ptr<Matrix<uint32_t>> input_img,
                           float target_row,
                           float target_col,
                           INTERPOLATION_TYPE interp_type);
std::shared_ptr<Matrix<uint32_t>> resize(std::shared_ptr<Matrix<uint32_t>> input_img,
                                         size_t target_rows,
                                         size_t target_cols,
                                         INTERPOLATION_TYPE interp_type = INTERPOLATION_TYPE::BILINEAR);
}

#endif