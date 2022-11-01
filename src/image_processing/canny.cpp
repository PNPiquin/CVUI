#include "image_processing/canny.h"

#include <cstdio>

#include "image_processing/morphological_operations.h"
#include "image_processing/spatial_filtering.h"
#include "image_processing/utils.h"

#include "utils/constants.h"

CannyEdgeDetector::CannyEdgeDetector(float low_threshold, float high_threshold)
  : low_threshold(low_threshold)
  , high_threshold(high_threshold)
{
  gaussian_kernel = ip::create_gaussian_kernel(5, 1.f);
  sobel_h = ip::create_sobel_h_kernel();
  sobel_v = ip::create_sobel_v_kernel();
}

void CannyEdgeDetector::process_rgba_img(std::shared_ptr<Matrix<uint32_t>> rgba_img)
{
  auto gray_img_ = ip::rgba_to_gray(rgba_img);
  process_gray_img(gray_img_);
}

void CannyEdgeDetector::process_gray_img(std::shared_ptr<Matrix<uint8_t>> gray_img_)
{
  gray_img = gray_img_;

  // First step, apply blur on input image
  auto blurred_img = std::make_shared<Matrix<uint8_t>>(gray_img->get_rows(), gray_img->get_cols());
  ip::apply_kernel(gray_img, gaussian_kernel, blurred_img);

  // Second step, apply v and h sobel filters
  edge_h = std::make_shared<Matrix<float>>(gray_img->get_rows(), gray_img->get_cols());
  edge_v = std::make_shared<Matrix<float>>(gray_img->get_rows(), gray_img->get_cols());
  ip::apply_kernel(blurred_img, sobel_h, edge_h);
  ip::apply_kernel(blurred_img, sobel_v, edge_v);

  // Third stap, extract magnitude and angle information from computed edges
  compute_edge_magnitude_and_angle();

  // Fourth step, compute the final raw canny edges
  compute_raw_canny_edges();

  // Final step, apply threshold on canny edges
  threshold_raw_canny_edges();
}

void CannyEdgeDetector::compute_edge_magnitude_and_angle()
{
  edge_magnitude = std::make_shared<Matrix<float>>(gray_img->get_rows(), gray_img->get_cols());
  edge_angle = std::make_shared<Matrix<float>>(gray_img->get_rows(), gray_img->get_cols());

  for (size_t row = 0; row < gray_img->get_rows(); row++) {
    for (size_t col = 0; col < gray_img->get_cols(); col++) {
      float gx = edge_h->operator()(row, col), gy = edge_v->operator()(row, col);
      edge_magnitude->operator()(row, col) = std::floor(std::sqrt(std::pow(gx, 2) + std::pow(gy, 2)) + 0.5);
      edge_angle->operator()(row, col) = std::atan(gy == 0.0 ? (gx / gy) : (1000 * gx));
    }
  }
}

void CannyEdgeDetector::compute_raw_canny_edges()
{
  raw_canny_edges = std::make_shared<Matrix<float>>(gray_img->get_rows(), gray_img->get_cols());
  for (size_t row = 1; row < gray_img->get_rows() - 1; row++) {
    for (size_t col = 1; col < gray_img->get_cols() - 1; col++) {
      float alpha = edge_angle->operator()(row, col);
      if ((alpha < (M_PI / 8.0)) && (alpha > (-M_PI / 8.0))) {
        if ((edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row, col - 1)) ||
            (edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row, col + 1))) {
          raw_canny_edges->operator()(row, col) = 0;
        } else {
          raw_canny_edges->operator()(row, col) = edge_magnitude->operator()(row, col);
        }
      } else if ((alpha < (3 * M_PI / 8.0)) && (alpha > (M_PI / 8.0))) {
        if ((edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row - 1, col - 1)) ||
            (edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row + 1, col + 1))) {
          raw_canny_edges->operator()(row, col) = 0;
        } else {
          raw_canny_edges->operator()(row, col) = edge_magnitude->operator()(row, col);
        }
      } else if ((alpha < (-M_PI / 8.0)) && (alpha > (-3 * M_PI / 8.0))) {
        if ((edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row + 1, col - 1)) ||
            (edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row - 1, col + 1))) {
          raw_canny_edges->operator()(row, col) = 0;
        } else {
          raw_canny_edges->operator()(row, col) = edge_magnitude->operator()(row, col);
        }
      } else {
        if ((edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row + 1, col)) ||
            (edge_magnitude->operator()(row, col) < edge_magnitude->operator()(row - 1, col))) {
          raw_canny_edges->operator()(row, col) = 0;
        } else {
          raw_canny_edges->operator()(row, col) = edge_magnitude->operator()(row, col);
        }
      }
    }
  }
}

void CannyEdgeDetector::threshold_raw_canny_edges()
{
  auto neighborhood_kernel = std::make_shared<Matrix<uint8_t>>(3, 3);
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      neighborhood_kernel->operator()(i, j) = 1;
    }
  }

  canny_edges = std::make_shared<Matrix<uint8_t>>(gray_img->get_rows(), gray_img->get_cols());
  for (size_t row = 0; row < raw_canny_edges->get_rows(); row++) {
    for (size_t col = 0; col < raw_canny_edges->get_cols(); col++) {
      float edge_value = raw_canny_edges->operator()(row, col);
      if (edge_value > high_threshold) {
        canny_edges->operator()(row, col) = uint8_t(edge_value);
      } else if (edge_value >= low_threshold) {
        if (ip::get_max(ip::extract_neighborhood(raw_canny_edges, neighborhood_kernel, row, col)) > high_threshold) {
          canny_edges->operator()(row, col) = uint8_t(edge_value);
        }
      }
    }
  }
}