#ifndef KMEANS_H
#define KMEANS_H

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "image_processing/base.h"
#include "utils/matrix.h"

class KMeans
{
public:
  enum K_MEANS_DISTANCE
  {
    EUCLIDIAN_DISTANCE,
    ED_SVD, // Euclidian distance + square value diff
    SVD     // square value diff
  };

  KMeans(int k = 2, K_MEANS_DISTANCE distance_method = EUCLIDIAN_DISTANCE, int max_steps = 25);
  void process_kmeans(std::shared_ptr<Matrix<uint8_t>> img, std::shared_ptr<Matrix<uint8_t>> img_out);

  void set_seeds(std::vector<Pixel> seeds);

private:
  int number_of_clusters;
  int max_steps;
  bool is_initialized;

  struct Cluster
  {
    double total_dist;
    Pixel cluster_center;
    std::vector<Pixel> pixels;
    Cluster() { total_dist = 0; }
    void clear_pixels() { pixels.clear(); }
  };

  std::map<int, Cluster> clusters;
  std::function<double(Pixel, Pixel)> distance;

  void init(std::shared_ptr<Matrix<uint8_t>> img,
            int x_min,
            int x_max,
            int y_min,
            int y_max); // Create k seeds in the area delimited by x_min, x_max, y_min, y_max
  double process_kmeans_step(std::shared_ptr<Matrix<uint8_t>> img);
  int find_closest_cluster(Pixel p, double& dist);
};

#endif // KMEANS_H
