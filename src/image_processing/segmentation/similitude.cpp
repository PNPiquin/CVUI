#include "image_processing/segmentation/similitude.h"

#include <cmath>
#include <iostream>

#include "image_processing/utils.h"
#include "utils/constants.h"

RegionSimilitude::RegionSimilitude(float similitude_threshold,
                                   int region_width,
                                   int region_height,
                                   int min_region_size,
                                   bool merge_regions,
                                   std::shared_ptr<Matrix<uint32_t>> rgba_img)
  : similitude_threshold(similitude_threshold)
  , region_width(region_width)
  , region_height(region_height)
  , min_region_size(min_region_size)
  , merge_regions(merge_regions)
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
    size_t n_regions = regions.size();
    size_t new_region_cpt = 0;
    for (auto& region : regions) {
      if (!region.already_processed) {
        check_homogenous_region_hsva(region);
      } else if (!region.is_homogenous) {
        int middle_row = region.row_min + (region.row_max - region.row_min) / 2;
        int middle_col = region.col_min + (region.col_max - region.col_min) / 2;

        HSVARegion top_left_region(region.row_min, middle_row, region.col_min, middle_col);
        top_left_region.top = region.top;
        top_left_region.left = region.left;
        top_left_region.right = n_regions + new_region_cpt + 1;
        top_left_region.bottom = n_regions + new_region_cpt + 2;
        check_homogenous_region_hsva(top_left_region);
        sub_regions.push_back(top_left_region);

        HSVARegion top_right_region(region.row_min, middle_row, middle_col, region.col_max);
        top_left_region.top = region.top;
        top_left_region.left = n_regions + new_region_cpt;
        top_left_region.right = region.right;
        top_left_region.bottom = n_regions + new_region_cpt + 3;
        check_homogenous_region_hsva(top_right_region);
        sub_regions.push_back(top_right_region);

        HSVARegion bottom_left_region(middle_row, region.row_max, region.col_min, middle_col);
        top_left_region.top = n_regions + new_region_cpt;
        top_left_region.left = region.left;
        top_left_region.right = n_regions + new_region_cpt + 3;
        top_left_region.bottom = region.bottom;
        check_homogenous_region_hsva(bottom_left_region);
        sub_regions.push_back(bottom_left_region);

        HSVARegion bottom_right_region(middle_row, region.row_max, middle_col, region.col_max);
        top_left_region.top = n_regions + new_region_cpt + 1;
        top_left_region.left = n_regions + new_region_cpt + 2;
        top_left_region.right = region.right;
        top_left_region.bottom = region.bottom;
        check_homogenous_region_hsva(bottom_right_region);
        sub_regions.push_back(bottom_right_region);
      }
    }
    for (auto& region : sub_regions) {
      regions.push_back(region);
    }

    current_region_minimum_size = current_region_minimum_size / 2;
  }

  // Try to merge regions
  if (merge_regions) {
    for (auto& region : regions) {
      if (region.is_homogenous) {
        if (region.top != -1 && regions[region.top].is_homogenous) {
          merge_region(region, regions[region.top]);
        }
        if (region.left != -1 && regions[region.left].is_homogenous) {
          merge_region(region, regions[region.left]);
        }
        if (region.bottom != -1 && regions[region.bottom].is_homogenous) {
          merge_region(region, regions[region.bottom]);
        }
        if (region.right != -1 && regions[region.right].is_homogenous) {
          merge_region(region, regions[region.right]);
        }
      }
    }
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

  // Setup locations indexes
  for (auto& region : regions) {
    for (size_t idx = 0; idx < regions.size(); ++idx) {
      HSVARegion region_to_check = regions[idx];
      if (region.col_min == region_to_check.col_min && region.col_max == region_to_check.col_max) {
        if (region.row_min == region_to_check.row_max) {
          // region_to_check is above region
          region.top = idx;
        }
        if (region.row_max == region_to_check.row_min) {
          // region_to_check is below region
          region.bottom = idx;
        }
      }

      if (region.row_min == region_to_check.row_min && region.row_max == region_to_check.row_max) {
        if (region.col_min == region_to_check.col_max) {
          // region_to_check is left of region
          region.left = idx;
        }
        if (region.col_max == region_to_check.col_min) {
          // region_to_check is right of region
          region.right = idx;
        }
      }
    }
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

float get_h_diff(float h1, float h2)
{
  float h_diff_raw = std::abs(h1 - h2);
  float h_diff = std::min(h_diff_raw, 255.f - h_diff_raw);
  return h_diff;
}

float get_h_merged(float h1, float h2)
{
  float h_diff = get_h_diff(h1, h2);
  float h_merged_1 = std::fmod(h1 + h_diff / 2.f, 255.f);
  float h_merged_2 = std::fmod(h2 - h_diff / 2.f, 255.f);

  // Check the difference between input values and the first merge candidate
  float h1_diff_m1 = get_h_diff(h1, h_merged_1);
  float h2_diff_m1 = get_h_diff(h2, h_merged_1);

  // Check the difference between input values and the second merge candidate
  float h1_diff_m2 = get_h_diff(h1, h_merged_2);
  float h2_diff_m2 = get_h_diff(h2, h_merged_2);

  if (h1_diff_m1 + h2_diff_m1 < h1_diff_m2 + h2_diff_m2) {
    return h_merged_1;
  } else {
    return h_merged_2;
  }
}

void RegionSimilitude::merge_region(HSVARegion& region_1, HSVARegion& region_2)
{
  // 0. We can only merge processed regions
  if (!region_1.already_processed || !region_2.already_processed) {
    return;
  }

  // 1. Check if region are similar. If not, no-op
  float h_diff = get_h_diff(float(region_1.pix.h), float(region_2.pix.h)) / 255.f;
  float s_diff = std::abs(float(region_1.pix.s) - float(region_2.pix.s)) / 255.f;
  float v_diff = std::abs(float(region_1.pix.v) - float(region_2.pix.v)) / 255.f;
  // To merge region, we check each term differently, Hue needs to be really close, value and saturation
  // a bit less
  if (h_diff > 0.5 * similitude_threshold) {
    return;
  }
  if (s_diff > similitude_threshold) {
    return;
  }
  if (v_diff > similitude_threshold) {
    return;
  }

  // // 2. Here region are similar, we can merge them
  // float h_merged = get_h_merged(float(region_1.pix.h), float(region_2.pix.h));
  // float s_merged = float(region_1.pix.s) + 0.5f * (float(region_2.pix.s) - float(region_1.pix.s));
  // float v_merged = float(region_1.pix.v) + 0.5f * (float(region_2.pix.v) - float(region_1.pix.v));
  // HSVAPixel merged_pixel = HSVAPixel(uint8_t(h_merged), uint8_t(s_merged), uint8_t(v_merged));
  RGBAPixel pix1 = region_1.pix.to_rgba_pixel();
  RGBAPixel pix2 = region_2.pix.to_rgba_pixel();
  HSVAPixel merged_pixel = HSVAPixel(RGBAPixel(uint8_t((float(pix1.r) + float(pix2.r)) / 2.f),
                                               uint8_t((float(pix1.g) + float(pix2.g)) / 2.f),
                                               uint8_t((float(pix1.b) + float(pix2.b)) / 2.f)));
  region_1.pix = merged_pixel;
  region_2.pix = merged_pixel;
}
