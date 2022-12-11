#include "image_processing/object_detection/blob_detection.h"

#include "utils/rtl/RTL.hpp"
#include <iostream>

Blob::Blob() {}

void Blob::compute_statistics()
{
  float min_x = 1e9f, max_x = 0.f, min_y = 1e9f, max_y = 0.f;
  std::vector<RTLPoint> rtl_points;
  for (const auto& point : points) {
    if (point.x < min_x) {
      min_x = point.x;
    }
    if (point.x > max_x) {
      max_x = point.x;
    }
    if (point.y < min_y) {
      min_y = point.y;
    }
    if (point.y > max_y) {
      max_y = point.y;
    }

    rtl_points.push_back(RTLPoint(point.x, point.y));
  }

  // Fill in SPAN stats
  x_span = max_x - min_x + 1.f;
  y_span = max_y - min_y + 1.f;
  xy_ratio = x_span / y_span;

  // Find the best model using RANSAC
  // LineEstimator estimator;
  // RTL::RANSAC<Line, RTLPoint, std::vector<RTLPoint>> ransac(&estimator);
  // Line model;
  // double loss = ransac.FindBest(model, rtl_points, rtl_points.size(), 2);

  // // Determine inliers using the best model if necessary
  // std::vector<int> inliers = ransac.FindInliers(model, rtl_points, rtl_points.size());

  // // Print the result
  // std::cout << "- Found Model: " << model.to_string() << " (Loss: " << loss << ")" << std::endl;
  // std::cout << "- The Number of Inliers: " << inliers.size() << " (N: " << rtl_points.size() << ")" << std::endl;
}

BlobDetector::BlobDetector(float x_min_span, float y_min_span, float min_xy_ratio, float max_xy_ratio)
  : x_min_span(x_min_span)
  , y_min_span(y_min_span)
  , min_xy_ratio(min_xy_ratio)
  , max_xy_ratio(max_xy_ratio)
{
}

std::shared_ptr<Matrix<uint8_t>> BlobDetector::process(std::shared_ptr<Matrix<uint8_t>> input_img)
{
  auto blob_img = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());

  auto processed_pixels = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());
  for (size_t row = 0; row < input_img->get_rows(); row++) {
    for (size_t col = 0; col < input_img->get_cols(); col++) {
      if (input_img->operator()(row, col) == 0) {
        processed_pixels->operator()(row, col) = 1;
      }
    }
  }

  for (size_t row = 0; row < input_img->get_rows(); row++) {
    for (size_t col = 0; col < input_img->get_cols(); col++) {
      if (processed_pixels->operator()(row, col) == 0) {
        extract_blob(input_img, processed_pixels, row, col);
      }
    }
  }

  // Compute blob statistics
  for (auto& blob : raw_blobs) {
    blob.compute_statistics();
  }

  // Keep only good blobs
  filter_blobs();

  // Fill in output blob image
  for (const auto& blob : blobs) {
    for (const auto& point : blob.points) {
      blob_img->operator()(size_t(point.x), size_t(point.y)) = 255;
    }
  }
  return blob_img;
}

void BlobDetector::extract_blob(std::shared_ptr<Matrix<uint8_t>> input_img,
                                std::shared_ptr<Matrix<uint8_t>> processed_pixels,
                                size_t row,
                                size_t col)
{
  Blob blob;
  std::vector<Point> points_to_check;
  points_to_check.push_back(Point(row, col));
  blob.points.push_back(Point(row, col));
  while (points_to_check.size() > 0) {
    auto current_point = points_to_check.back();
    size_t current_row = current_point.x, current_col = current_point.y;
    points_to_check.pop_back();

    // Top pixel
    if (current_row < input_img->get_rows() - 1) {
      if (input_img->operator()(current_row + 1, current_col) > 0 &&
          processed_pixels->operator()(current_row + 1, current_col) == 0) {
        points_to_check.push_back(Point(current_row + 1, current_col));
        blob.points.push_back(Point(current_row + 1, current_col));
        processed_pixels->operator()(current_row + 1, current_col) = 1;
      }
    }

    // Bottom pixel
    if (current_row > 0) {
      if (input_img->operator()(current_row - 1, current_col) > 0 &&
          processed_pixels->operator()(current_row - 1, current_col) == 0) {
        points_to_check.push_back(Point(current_row - 1, current_col));
        blob.points.push_back(Point(current_row - 1, current_col));
        processed_pixels->operator()(current_row - 1, current_col) = 1;
      }
    }

    // Left pixel
    if (current_col > 0) {
      if (input_img->operator()(current_row, current_col - 1) > 0 &&
          processed_pixels->operator()(current_row, current_col - 1) == 0) {
        points_to_check.push_back(Point(current_row, current_col - 1));
        blob.points.push_back(Point(current_row, current_col - 1));
        processed_pixels->operator()(current_row, current_col - 1) = 1;
      }
    }

    // Right pixel
    if (current_col < input_img->get_cols() - 1) {
      if (input_img->operator()(current_row, current_col + 1) > 0 &&
          processed_pixels->operator()(current_row, current_col + 1) == 0) {
        points_to_check.push_back(Point(current_row, current_col + 1));
        blob.points.push_back(Point(current_row, current_col + 1));
        processed_pixels->operator()(current_row, current_col + 1) = 1;
      }
    }
  }

  raw_blobs.push_back(blob);
}

void BlobDetector::filter_blobs()
{
  for (const auto& blob : raw_blobs) {
    if (blob.x_span < x_min_span) {
      continue;
    }
    if (blob.y_span < y_min_span) {
      continue;
    }

    if (blob.xy_ratio < min_xy_ratio) {
      continue;
    }
    if (blob.xy_ratio > max_xy_ratio) {
      continue;
    }

    blobs.push_back(blob);
  }
}
