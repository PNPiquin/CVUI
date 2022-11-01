#ifndef IMAGE_PROCESSING_CANNY_H
#define IMAGE_PROCESSING_CANNY_H

#include <cstdint>
#include <memory>

#include "image_processing/spatial_filtering.h"
#include "utils/matrix.h"

class CannyEdgeDetector
{
public:
  CannyEdgeDetector(float low_threshold, float high_threshold);
  ~CannyEdgeDetector() = default;

  void process_rgba_img(std::shared_ptr<Matrix<uint32_t>> rgba_img);
  void process_gray_img(std::shared_ptr<Matrix<uint8_t>> gray_img_);

  std::shared_ptr<Matrix<uint8_t>> get_canny_edges() { return canny_edges; };

private:
  // Canny configuration
  float low_threshold, high_threshold;

  // Canny process image
  std::shared_ptr<Matrix<float>> sobel_h, sobel_v, gaussian_kernel;
  std::shared_ptr<Matrix<uint8_t>> gray_img;
  std::shared_ptr<Matrix<float>> edge_h, edge_v;
  std::shared_ptr<Matrix<float>> edge_magnitude, edge_angle;
  std::shared_ptr<Matrix<float>> raw_canny_edges;
  std::shared_ptr<Matrix<uint8_t>> canny_edges;

  void compute_edge_magnitude_and_angle();
  void compute_raw_canny_edges();
  void threshold_raw_canny_edges();
};

#endif