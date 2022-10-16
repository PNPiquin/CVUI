#include "image_processing/segmentation/similitude.h"

#include <cmath>
#include <iostream>

#include "image_processing/utils.h"
#include "utils/constants.h"

RegionSimilitude::RegionSimilitude(float similitude_threshold,
                                   int region_width,
                                   int region_height,
                                   int min_region_size,
                                   std::shared_ptr<Matrix<uint32_t>> rgba_img)
  : similitude_threshold(similitude_threshold)
  , region_width(region_width)
  , region_height(region_height)
  , min_region_size(min_region_size)
  , rgba_img(rgba_img)
{
  hsva_img = ip::rgba_to_hsva(rgba_img);
  res_img = std::make_shared<Matrix<uint32_t>>(rgba_img->get_rows(), rgba_img->get_cols());
}

std::shared_ptr<Matrix<uint32_t>> RegionSimilitude::process()
{
  // Create list of region to process
  create_regions();

  // Check for each region homogeneousness
  int current_region_minimum_size = std::min(region_width, region_height);
  while (current_region_minimum_size > min_region_size) {
    printf("Similitude iteration: %llu regions -- min size %d\n", regions.size(), current_region_minimum_size);
    std::vector<HSVARegion> sub_regions;
    for (auto& region : regions) {
      if (!region.already_processed) {
        check_homogenous_region_hsva(region);
      } else if (!region.is_homogenous) {
        int middle_row = region.row_min + (region.row_max - region.row_min) / 2;
        int middle_col = region.col_min + (region.col_max - region.col_min) / 2;

        HSVARegion top_left_region(region.row_min, middle_row, region.col_min, middle_col);
        check_homogenous_region_hsva(top_left_region);
        sub_regions.push_back(top_left_region);

        HSVARegion top_right_region(region.row_min, middle_row, middle_col, region.col_max);
        check_homogenous_region_hsva(top_right_region);
        sub_regions.push_back(top_right_region);

        HSVARegion bottom_left_region(middle_row, region.row_max, region.col_min, middle_col);
        check_homogenous_region_hsva(bottom_left_region);
        sub_regions.push_back(bottom_left_region);

        HSVARegion bottom_right_region(middle_row, region.row_max, middle_col, region.col_max);
        check_homogenous_region_hsva(bottom_right_region);
        sub_regions.push_back(bottom_right_region);
      }
    }
    for (auto& region : sub_regions) {
      regions.push_back(region);
    }

    current_region_minimum_size = current_region_minimum_size / 2;
  }

  // Fill output image in homogeneous regions
  for (auto& region : regions) {
    if (region.is_homogenous) {
      paint_region(region);
    }
  }

  return res_img;
}

void RegionSimilitude::create_regions()
{
  size_t grid_cols = rgba_img->get_cols() / region_width;
  size_t grid_rows = rgba_img->get_rows() / region_height;
  for (size_t row = 0; row < grid_rows - 1; ++row) {
    for (size_t col = 0; col < grid_cols - 1; ++col) {
      regions.push_back(
        HSVARegion(row * region_height, (row + 1) * region_height, col * region_width, (col + 1) * region_width));
    }
    if ((grid_cols - 1) * region_width < rgba_img->get_cols()) {
      regions.push_back(HSVARegion(
        row * region_height, (row + 1) * region_height, (grid_cols - 1) * region_width, rgba_img->get_cols()));
    }
  }
  if ((grid_rows - 1) * region_height < rgba_img->get_rows()) {
    for (size_t col = 0; col < grid_cols - 1; ++col) {
      regions.push_back(HSVARegion(
        (grid_rows - 1) * region_height, rgba_img->get_rows(), col * region_width, (col + 1) * region_width));
    }
  }
  if ((grid_cols - 1) * region_width < rgba_img->get_cols() && (grid_rows - 1) * region_height < rgba_img->get_rows()) {
    regions.push_back(HSVARegion(
      (grid_rows - 1) * region_height, rgba_img->get_rows(), (grid_cols - 1) * region_width, rgba_img->get_cols()));
  }
}

void RegionSimilitude::check_homogenous_region_hsva(HSVARegion& region)
{
  region.already_processed = true;
  std::vector<uint8_t> hue_vect, sat_vect, val_vect;
  for (size_t row = region.row_min; row < region.row_max; ++row) {
    for (size_t col = region.col_min; col < region.col_max; ++col) {
      HSVAPixel pix(hsva_img->operator()(row, col));
      hue_vect.push_back(pix.h);
      sat_vect.push_back(pix.s);
      val_vect.push_back(pix.v);
    }
  }

  // Compute region stats
  float s_mean = 0.f, v_mean = 0.f;
  for (size_t idx = 0; idx < sat_vect.size(); ++idx) {
    s_mean += sat_vect[idx];
    v_mean += val_vect[idx];
  }
  s_mean = s_mean / sat_vect.size();
  v_mean = v_mean / sat_vect.size();
  float h_mean = compute_hue_mean(hue_vect);
  region.pix = HSVAPixel(uint8_t(h_mean), uint8_t(s_mean), uint8_t(v_mean));

  for (size_t row = region.row_min; row < region.row_max; ++row) {
    for (size_t col = region.col_min; col < region.col_max; ++col) {
      HSVAPixel pix(hsva_img->operator()(row, col));
      float h_dist = std::abs(float(pix.h) - h_mean);
      h_dist = std::min(h_dist, 255.f - h_dist) / 255.f;
      float dist =
        (1.f / 3.f) *
        std::sqrt(std::pow(h_dist + std::abs(float(pix.s) - s_mean) / 255.f + std::abs(pix.v - v_mean) / 255.f, 2.f));
      if (dist > similitude_threshold) {
        region.is_homogenous = false;
        return;
      }
    }
  }
  region.is_homogenous = true;
}

void RegionSimilitude::paint_region(HSVARegion& region)
{
  for (size_t row = region.row_min; row < region.row_max; ++row) {
    for (size_t col = region.col_min; col < region.col_max; ++col) {
      res_img->operator()(row, col) = region.pix.to_rgba_pixel().to_uint32_t();
    }
  }
}

// Utils
float RegionSimilitude::compute_hue_mean(std::vector<uint8_t> hue_vect)
{
  float h_mean = 0.f;
  for (size_t i = 0; i < 5; i++) {
    float ref = h_mean;
    float dist_to_mean = 0.f;
    bool always_reverted = true;
    for (size_t idx = 0; idx < hue_vect.size(); ++idx) {
      float hue_in_ref = float(hue_vect[idx]) - ref;
      if (std::abs(hue_in_ref) < std::abs(255.f - hue_in_ref)) {
        dist_to_mean = hue_in_ref;

        always_reverted = false;
      } else {
        dist_to_mean = 255.f - hue_in_ref;
      }
      h_mean += dist_to_mean;
    }
    if (always_reverted) {
      h_mean = 255.f - h_mean / hue_vect.size();
    } else {
      h_mean = h_mean / hue_vect.size();
    }
    h_mean = std::fmod(h_mean, 255.f);
  }
  return h_mean;
}
