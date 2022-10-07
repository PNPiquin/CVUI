#include "image_processing/segmentation.h"

#include "image_processing/kmeans.h"
#include "image_processing/utils.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> get_random_framing(std::shared_ptr<Matrix<uint32_t>> img, int n_tiles)
{
  std::shared_ptr<Matrix<uint32_t>> segmented_img =
    std::make_shared<Matrix<uint32_t>>(img->get_rows(), img->get_cols());

  KMeans kmeans(n_tiles, K_MEANS_DISTANCE::EUCLIDIAN_DISTANCE, 25);
  kmeans.process_kmeans(img, segmented_img);

  return ip::rgba_to_gray(segmented_img);
}
}
