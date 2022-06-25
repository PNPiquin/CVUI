#include "image_processing/segmentation.h"

#include "image_processing/kmeans.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_random_framing(std::shared_ptr<Matrix<uint8_t>> img, int n_tiles)
{
  std::shared_ptr<Matrix<uint8_t>> segmented_img = std::make_shared<Matrix<uint8_t>>(img->get_rows(), img->get_cols());

  KMeans kmeans(n_tiles, KMeans::EUCLIDIAN_DISTANCE, 25);
  kmeans.process_kmeans(img, segmented_img);

  return segmented_img;
}
}
