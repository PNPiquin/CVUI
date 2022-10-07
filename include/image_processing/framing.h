#ifndef FRAMING_H
#define FRAMING_H

#include <cstdint>
#include <memory>
#include <vector>

#include "image_processing/base.h"
#include "image_processing/kmeans.h"
#include "utils/matrix.h"

struct FramingConfiguration
{
  // Define starting grid
  int rows;
  int cols;

  // Whether to nudge the seeds around their starting positions
  bool randomize;
  int row_tolerance;
  int col_tolerance;
};

class FramingService
{
  // Framing is based on creating seeds, that will generate zones through a one-step KMeans
public:
  FramingService(FramingConfiguration config);
  ~FramingService() = default;

  std::shared_ptr<Matrix<uint8_t>> create_zones(std::shared_ptr<Matrix<uint8_t>> img);

private:
  FramingConfiguration config;
  KMeans kmeans;

  std::vector<Pixel<uint32_t>> seeds;

  void generate_starting_grid(int img_height, int img_width);
  void randomize_grid();
};

#endif