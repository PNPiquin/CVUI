#include "image_processing/bilateral_filtering.h"
#include <cstdio>

BilateralFilter::BilateralFilter(int kernel_size, float mu, float sigma)
  : kernel_size(kernel_size)
{
  normal_distribution = NormalDistribution(mu, sigma);
}

RGBAPixel BilateralFilter::apply_on_pixel(std::shared_ptr<Matrix<uint32_t>> input_img, size_t row, size_t col)
{
  Point3D acc(0.f, 0.f, 0.f);
  float w = 0.f;

  size_t min_row = std::max(0, int(row) - kernel_size),
         max_row = std::min(int(input_img->get_rows()), int(row + kernel_size + 1));
  size_t min_col = std::max(0, int(col) - kernel_size),
         max_col = std::min(int(input_img->get_cols()), int(col + kernel_size + 1));
  float gs, fr, current_weight;
  RGBAPixel rgba_pixel = RGBAPixel(input_img->operator()(row, col));
  for (size_t row_itr = min_row; row_itr < max_row; row_itr++) {
    for (size_t col_itr = min_col; col_itr < max_col; col_itr++) {
      RGBAPixel rgba_itr = RGBAPixel(input_img->operator()(row_itr, col_itr));
      gs = normal_distribution(Point(row, col).distance_to(Point(row_itr, col_itr)));
      fr = normal_distribution(
        Point3D(rgba_pixel.r, rgba_pixel.g, rgba_pixel.b).distance_to(Point3D(rgba_itr.r, rgba_itr.g, rgba_itr.b)));
      current_weight = fr * gs;
      w += current_weight;
      acc += Point3D(rgba_itr.r * current_weight, rgba_itr.g * current_weight, rgba_itr.b * current_weight);
    }
  }

  return RGBAPixel(uint8_t(acc.x / w), uint8_t(acc.y / w), uint8_t(acc.z / w));
}

std::shared_ptr<Matrix<uint32_t>> BilateralFilter::apply_on_img(std::shared_ptr<Matrix<uint32_t>> input_img)
{
  auto res_img = std::make_shared<Matrix<uint32_t>>(input_img->get_rows(), input_img->get_cols());

  for (size_t row = 0; row < input_img->get_rows(); row++) {
    for (size_t col = 0; col < input_img->get_cols(); col++) {
      res_img->operator()(row, col) = apply_on_pixel(input_img, row, col).to_uint32_t();
    }
    if (row % (input_img->get_rows() / 10) == 0) {
      printf("Processing row %d/%d\n", int(row), int(input_img->get_rows()));
    }
  }

  return res_img;
}
