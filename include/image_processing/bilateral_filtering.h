#ifndef IMAGE_PROCESSING_BILATERAL_FILTERING_H
#define IMAGE_PROCESSING_BILATERAL_FILTERING_H

#include <cstdint>
#include <memory>

#include "image_processing/utils.h"
#include "utils/math.h"
#include "utils/matrix.h"


class BilateralFilter
{
public:
  BilateralFilter(int kernel_size, float mu, float sigma);
  ~BilateralFilter() = default;

  std::shared_ptr<Matrix<uint32_t>> apply_on_img(std::shared_ptr<Matrix<uint32_t>> input_img);

private:
  int kernel_size;
  NormalDistribution normal_distribution;

  RGBAPixel apply_on_pixel(std::shared_ptr<Matrix<uint32_t>> input_img, size_t row, size_t col);
};

#endif