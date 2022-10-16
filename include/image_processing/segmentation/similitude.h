#ifndef IMAGE_PROCESSING_SEGMENTATION_SIMILITUDE_H
#define IMAGE_PROCESSING_SEGMENTATION_SIMILITUDE_H

#include <cstdint>
#include <memory>
#include <vector>

#include "image_processing/utils.h"
#include "utils/matrix.h"

struct HSVARegion
{
  size_t row_min, row_max, col_min, col_max;
  bool is_homogenous, already_processed;

  HSVAPixel pix;

  HSVARegion(size_t _row_min, size_t _row_max, size_t _col_min, size_t _col_max)
  {
    row_min = _row_min;
    row_max = _row_max;
    col_min = _col_min;
    col_max = _col_max;

    is_homogenous = false;
    already_processed = false;
  }
};

class RegionSimilitude
{
public:
  RegionSimilitude(float similitude_threshold,
                   int region_width,
                   int region_height,
                   int min_region_size,
                   std::shared_ptr<Matrix<uint32_t>> rgba_img);
  ~RegionSimilitude() = default;

  std::shared_ptr<Matrix<uint32_t>> process();
  std::shared_ptr<Matrix<uint32_t>> get_res_img() { return res_img; };

private:
  float similitude_threshold;
  int region_width, region_height, min_region_size;

  std::shared_ptr<Matrix<uint32_t>> rgba_img;
  std::shared_ptr<Matrix<uint32_t>> hsva_img;
  std::shared_ptr<Matrix<uint32_t>> res_img;

  std::vector<HSVARegion> regions;

  void create_regions();
  void check_homogenous_region_hsva(HSVARegion& region);
  void paint_region(HSVARegion& region);

  // Utils
  float compute_hue_mean(std::vector<uint8_t> hue_vect);
};

#endif