#ifndef KMEANS_H
#define KMEANS_H

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "image_processing/base.h"
#include "utils/matrix.h"

enum K_MEANS_DISTANCE
{
  EUCLIDIAN_DISTANCE,
  ED_SVD,     // Euclidian distance + square value diff
  ED_HSV_SVD, // Euclidian distance + square value diff
  SVD,        // square value diff
  HSV_SVD     // square diff value in HSV space
};

const std::map<K_MEANS_DISTANCE, std::string> K_MEANS_DISTANCE_NAMES = { { EUCLIDIAN_DISTANCE, "Euclidian distance" },
                                                                         { SVD, "RGB Squared diff" },
                                                                         { HSV_SVD, "HSV Squared diff" },
                                                                         { ED_SVD, "ED + RGB Squared diff" },
                                                                         { ED_HSV_SVD, "ED + HSV Squared diff" } };

const std::map<std::string, K_MEANS_DISTANCE> K_MEANS_DISTANCE_NAMES_TO_ENUM = {
  { "Euclidian distance", EUCLIDIAN_DISTANCE },
  { "RGB Squared diff", SVD },
  { "HSV Squared diff", HSV_SVD },
  { "ED + RGB Squared diff", ED_SVD },
  { "ED + HSV Squared diff", ED_HSV_SVD }
};

class KMeans
{
public:
  KMeans(int k = 2, K_MEANS_DISTANCE distance_method = EUCLIDIAN_DISTANCE, int max_steps = 25);
  void process_kmeans(std::shared_ptr<Matrix<uint32_t>> img, std::shared_ptr<Matrix<uint32_t>> img_out);

  void set_seeds(std::vector<Pixel<uint32_t>> seeds);

private:
  int number_of_clusters;
  int max_steps;
  bool is_initialized;

  struct Cluster
  {
    double total_dist;
    Pixel<uint32_t> cluster_center;
    std::vector<Pixel<uint32_t>> pixels;
    Cluster() { total_dist = 0; }
    void clear_pixels() { pixels.clear(); }
  };

  std::map<int, Cluster> clusters;
  std::function<double(Pixel<uint32_t>, Pixel<uint32_t>)> distance;

  void init(std::shared_ptr<Matrix<uint32_t>> img,
            int x_min,
            int x_max,
            int y_min,
            int y_max); // Create k seeds in the area delimited by x_min, x_max, y_min, y_max
  double process_kmeans_step(std::shared_ptr<Matrix<uint32_t>> img);
  int find_closest_cluster(Pixel<uint32_t> p, double& dist);
};

#endif // KMEANS_H
