#ifndef IMAGE_PROCESSING_OBJECT_DETECTION_H
#define IMAGE_PROCESSING_OBJECT_DETECTION_H

#include <memory>
#include <vector>

#include "utils/math.h"
#include "utils/matrix.h"

struct Blob
{
  std::vector<Point> points;

  // Statistics
  float area = 0.f;
  float perimeter = 0.f;
  float large_axis = 0.f;
  float small_axis = 0.f;
  float x_span = 0.f;
  float y_span = 0.f;
  float xy_ratio = 0.f;
  float theta = 0.f; // Angle between the large axis and the horizontal

  Blob();

  void compute_statistics();
};

class BlobDetector
{
public:
  BlobDetector(float x_min_span = 0.f, float y_min_span = 0.f, float min_xy_ratio = 0.f, float max_xy_ratio = 1e6f);
  ~BlobDetector() = default;

  std::shared_ptr<Matrix<uint8_t>> process(std::shared_ptr<Matrix<uint8_t>> input_img);

private:
  std::vector<Blob> raw_blobs, blobs;

  // Filtering properties
  float x_min_span, y_min_span, min_xy_ratio, max_xy_ratio;

  void extract_blob(std::shared_ptr<Matrix<uint8_t>> input_img,
                    std::shared_ptr<Matrix<uint8_t>> processed_pixels,
                    size_t row,
                    size_t col);

  void filter_blobs();
};

#endif