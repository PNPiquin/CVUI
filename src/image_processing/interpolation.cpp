#include "image_processing/interpolation.h"

#include <cmath>
#include <cstdio>

namespace ip {
uint8_t interpolate_pixel_bilinear(uint8_t nw_pix,
                                   uint8_t ne_pix,
                                   uint8_t sw_pix,
                                   uint8_t se_pix,
                                   float row_fraction,
                                   float col_fraction)
{
  float interpolated_value = (nw_pix * (1.f - row_fraction) * (1.f - col_fraction)) +
                             (ne_pix * row_fraction * (1.f - col_fraction)) +
                             (sw_pix * (1.f - row_fraction) * col_fraction) + (se_pix * row_fraction * col_fraction);
  interpolated_value /= ((1.f - row_fraction) * (1.f - col_fraction) + row_fraction * (1.f - col_fraction) +
                         (1.f - row_fraction) * col_fraction + row_fraction * col_fraction);
  return uint8_t(interpolated_value);
}

uint8_t interpolate_pixel(std::shared_ptr<Matrix<uint8_t>> input_img,
                          float target_row,
                          float target_col,
                          INTERPOLATION_TYPE interp_type)
{
  if (interp_type == INTERPOLATION_TYPE::BILINEAR) {
    // Build bilinear neighborhood
    size_t base_row = std::floor(target_row);
    size_t base_col = std::floor(target_col);

    size_t max_row = std::min(input_img->get_rows() - 1, base_row + 1);
    size_t max_col = std::min(input_img->get_cols() - 1, base_col + 1);

    return interpolate_pixel_bilinear(input_img->operator()(base_row, base_col),
                                      input_img->operator()(base_row, max_col),
                                      input_img->operator()(max_row, base_col),
                                      input_img->operator()(max_row, max_col),
                                      target_row - base_row,
                                      target_col - base_col);
  } else {
    return 0;
  }
}

std::shared_ptr<Matrix<uint8_t>> resize(std::shared_ptr<Matrix<uint8_t>> input_img,
                                        size_t target_rows,
                                        size_t target_cols,
                                        INTERPOLATION_TYPE interp_type)
{
  auto output_img = std::make_shared<Matrix<uint8_t>>(target_rows, target_cols);

  float row_ratio = float(input_img->get_rows()) / float(target_rows);
  float col_ratio = float(input_img->get_cols()) / float(target_cols);
  for (size_t row = 0; row < target_rows; row++) {
    for (size_t col = 0; col < target_cols; col++) {
      float target_row = float(row) * row_ratio;
      float target_col = float(col) * col_ratio;
      output_img->operator()(row, col) = interpolate_pixel(input_img, target_row, target_col, interp_type);
    }
  }

  return output_img;
}

RGBAPixel interpolate_pixel_bilinear(RGBAPixel nw_pix,
                                     RGBAPixel ne_pix,
                                     RGBAPixel sw_pix,
                                     RGBAPixel se_pix,
                                     float row_fraction,
                                     float col_fraction)
{
  return RGBAPixel(interpolate_pixel_bilinear(nw_pix.r, ne_pix.r, sw_pix.r, se_pix.r, row_fraction, col_fraction),
                   interpolate_pixel_bilinear(nw_pix.g, ne_pix.g, sw_pix.g, se_pix.g, row_fraction, col_fraction),
                   interpolate_pixel_bilinear(nw_pix.b, ne_pix.b, sw_pix.b, se_pix.b, row_fraction, col_fraction));
}

uint32_t interpolate_pixel(std::shared_ptr<Matrix<uint32_t>> input_img,
                           float target_row,
                           float target_col,
                           INTERPOLATION_TYPE interp_type)
{
  if (interp_type == INTERPOLATION_TYPE::BILINEAR) {
    // Build bilinear neighborhood
    size_t base_row = std::floor(target_row);
    size_t base_col = std::floor(target_col);

    size_t max_row = std::min(input_img->get_rows() - 1, base_row + 1);
    size_t max_col = std::min(input_img->get_cols() - 1, base_col + 1);

    return interpolate_pixel_bilinear(RGBAPixel(input_img->operator()(base_row, base_col)),
                                      RGBAPixel(input_img->operator()(base_row, max_col)),
                                      RGBAPixel(input_img->operator()(max_row, base_col)),
                                      RGBAPixel(input_img->operator()(max_row, max_col)),
                                      target_row - base_row,
                                      target_col - base_col)
      .to_uint32_t();
  } else {
    return 0;
  }
}

std::shared_ptr<Matrix<uint32_t>> resize(std::shared_ptr<Matrix<uint32_t>> input_img,
                                         size_t target_rows,
                                         size_t target_cols,
                                         INTERPOLATION_TYPE interp_type)
{
  auto output_img = std::make_shared<Matrix<uint32_t>>(target_rows, target_cols);

  float row_ratio = float(input_img->get_rows()) / float(target_rows);
  float col_ratio = float(input_img->get_cols()) / float(target_cols);
  for (size_t row = 0; row < target_rows; row++) {
    for (size_t col = 0; col < target_cols; col++) {
      float target_row = float(row) * row_ratio;
      float target_col = float(col) * col_ratio;
      output_img->operator()(row, col) = interpolate_pixel(input_img, target_row, target_col, interp_type);
    }
  }

  return output_img;
}
}