#include "image_processing/kmeans.h"
#include "image_processing/utils.h"
#include <random>

KMeans::KMeans(int k, K_MEANS_DISTANCE distance_method, int max_steps)
  : number_of_clusters(k)
  , max_steps(max_steps)
  , is_initialized(false)
{
  for (int i = 0; i < number_of_clusters; ++i) {
    clusters.insert(std::pair<int, Cluster>(i, Cluster()));
  }
  switch (distance_method) {
    case ED_SVD:
      distance = [](Pixel<uint32_t> p1, Pixel<uint32_t> p2) {
        double euclidian_distance = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)) / 1000.;
        RGBAPixel _p1(p1.value), _p2(p2.value);
        double value_dist =
          std::sqrt(std::pow(_p1.r - _p2.r, 2) + std::pow(_p1.g - _p2.g, 2) + std::pow(_p1.b - _p2.b, 2)) / 255.;
        return (euclidian_distance + value_dist) / 2;
      };
      break;
    case ED_HSV_SVD:
      distance = [](Pixel<uint32_t> p1, Pixel<uint32_t> p2) {
        double euclidian_distance = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2)) / 1000.;
        HSVAPixel _p1(p1.value), _p2(p2.value);
        double value_dist = std::sqrt(std::pow(_p1.h - _p2.h, 2) + 0.25f * std::pow(_p1.s - _p2.s, 2) +
                                      0.25f * std::pow(_p1.v - _p2.v, 2)) /
                            255.;
        return (euclidian_distance + value_dist) / 2;
      };
      break;
    case EUCLIDIAN_DISTANCE:
      distance = [](Pixel<uint32_t> p1, Pixel<uint32_t> p2) {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
      };
      break;
    case SVD:
      distance = [](Pixel<uint32_t> p1, Pixel<uint32_t> p2) {
        RGBAPixel _p1(p1.value), _p2(p2.value);
        return std::sqrt(std::pow(_p1.r - _p2.r, 2) + std::pow(_p1.g - _p2.g, 2) + std::pow(_p1.b - _p2.b, 2));
      };
      break;
    case HSV_SVD:
      distance = [](Pixel<uint32_t> p1, Pixel<uint32_t> p2) {
        HSVAPixel _p1(p1.value), _p2(p2.value);
        return std::sqrt(std::pow(_p1.h - _p2.h, 2) + 0.25f * std::pow(_p1.s - _p2.s, 2) +
                         0.25f * std::pow(_p1.v - _p2.v, 2));
      };
      break;
  }
}

void KMeans::process_kmeans(std::shared_ptr<Matrix<uint32_t>> img, std::shared_ptr<Matrix<uint32_t>> img_out)
{
  int rows = img->get_rows();
  int cols = img->get_cols();

  // Initialization of cluster centers
  if (!is_initialized) {
    init(img, 0, rows - 1, 0, cols - 1);
  }

  int iter(0);
  double epsilon(1.0), prev_value(0.0);
  double total_value = -epsilon - 1.0;
  while (std::abs(total_value - prev_value) > epsilon && iter < max_steps) {
    prev_value = total_value;
    ++iter;
    total_value = process_kmeans_step(img);
    total_value /= (rows * cols);
    printf("iter : %d || delta : %f \n", iter, std::abs(total_value - prev_value));
  }

  // int color_inc = int(255 / (number_of_clusters - 1));
  for (int index = 0; index < number_of_clusters; ++index) {
    // int gray_color = index * color_inc;
    // RGBAPixel pix(gray_color, gray_color, gray_color);
    uint32_t color = clusters.at(index).cluster_center.value;
    for (const auto& px : clusters.at(index).pixels) {
      img_out->operator()(px.x, px.y) = color;
    }
  }
}

void KMeans::set_seeds(std::vector<Pixel<uint32_t>> seeds)
{
  if (int(seeds.size()) != number_of_clusters) {
    number_of_clusters = int(seeds.size());
    for (int i = 0; i < number_of_clusters; ++i) {
      clusters.insert(std::pair<int, Cluster>(i, Cluster()));
    }
  }

  for (int i = 0; i < number_of_clusters; ++i) {
    const auto seed = seeds[i];
    clusters.at(i).cluster_center.set_coord(seed.x, seed.y);
    clusters.at(i).cluster_center.set_value(seed.value);
  }

  is_initialized = true;
}

void KMeans::init(std::shared_ptr<Matrix<uint32_t>> img, int x_min, int x_max, int y_min, int y_max)
{
  std::default_random_engine x_generator, y_generator;
  std::uniform_int_distribution<int> x_distribution(x_min, x_max);
  std::uniform_int_distribution<int> y_distribution(y_min, y_max);
  auto x_gen = std::bind(x_distribution, x_generator);
  auto y_gen = std::bind(y_distribution, y_generator);
  for (int i = 0; i < number_of_clusters; ++i) {
    clusters.at(i).cluster_center.set_coord(x_gen(), y_gen());
    clusters.at(i).cluster_center.update_values(img);
  }

  is_initialized = true;
}

double KMeans::process_kmeans_step(std::shared_ptr<Matrix<uint32_t>> img)
{
  int rows = img->get_rows();
  int cols = img->get_cols();

  for (int index = 0; index < number_of_clusters; ++index) {
    clusters.at(index).clear_pixels();
    clusters.at(index).total_dist = 0.0;
  }

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double dist;
      Pixel<uint32_t> px(i, j, img->operator()(i, j));
      int cluster_index = find_closest_cluster(px, dist);
      clusters.at(cluster_index).pixels.push_back(px);
      clusters.at(cluster_index).total_dist += dist;
    }
  }

  double total_dist(0.0);
  for (int index = 0; index < number_of_clusters; ++index) {
    total_dist += clusters.at(index).total_dist;
    int x_mean(0), y_mean(0), n(0);
    for (const auto& px : clusters.at(index).pixels) {
      ++n;
      x_mean += px.x;
      y_mean += px.y;
    }
    int x_center = n > 0 ? int(x_mean / n) : clusters.at(index).cluster_center.x;
    int y_center = n > 0 ? int(y_mean / n) : clusters.at(index).cluster_center.y;
    x_center = x_center >= rows ? rows - 1 : x_center;
    x_center = x_center < 0 ? 0 : x_center;
    y_center = y_center >= cols ? cols - 1 : y_center;
    y_center = y_center < 0 ? 0 : y_center;
    clusters.at(index).cluster_center.set_coord(x_center, y_center);
    clusters.at(index).cluster_center.update_values(img);
  }
  return total_dist;
}

int KMeans::find_closest_cluster(Pixel<uint32_t> p, double& dist)
{
  int cluster_index = 0;
  double min_dist = std::numeric_limits<double>::max();

  for (int i = 0; i < number_of_clusters; ++i) {
    double current_dist = distance(p, clusters.at(i).cluster_center);
    if (current_dist < min_dist) {
      min_dist = current_dist;
      cluster_index = i;
    }
  }
  dist = min_dist;
  return cluster_index;
}
