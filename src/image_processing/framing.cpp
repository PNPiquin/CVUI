#include "image_processing/framing.h"

#include <random>

FramingService::FramingService(FramingConfiguration config)
  : config(config)
{
  seeds.reserve(config.rows * config.cols);
  kmeans = KMeans(config.rows * config.cols, KMeans::EUCLIDIAN_DISTANCE, 1);
}

std::shared_ptr<Matrix<uint8_t>> FramingService::create_zones(std::shared_ptr<Matrix<uint8_t>> img)
{
  // Generate seeds
  generate_starting_grid(img->get_rows(), img->get_cols());
  if (config.randomize) {
    randomize_grid();
  }
  kmeans.set_seeds(seeds);

  // Use kmeans to generate zones
  auto zone_img = std::make_shared<Matrix<uint8_t>>(img->get_rows(), img->get_cols());
  kmeans.process_kmeans(img, zone_img);
  return zone_img;
}

void FramingService::generate_starting_grid(int img_height, int img_width)
{
  int row_step = img_height / config.rows;
  int row_semi_step = row_step / 2;

  int col_step = img_width / config.cols;
  int col_semi_step = col_step / 2;

  for (int row = 0; row < config.rows; ++row) {
    for (int col = 0; col < config.cols; ++col) {
      seeds.push_back(Pixel(row_semi_step + row * row_step, col_semi_step + col * col_step, 0));
    }
  }
}

void FramingService::randomize_grid()
{
  std::default_random_engine x_generator, y_generator;
  std::uniform_int_distribution<int> x_distribution(-config.row_tolerance, config.row_tolerance);
  std::uniform_int_distribution<int> y_distribution(-config.col_tolerance, config.col_tolerance);
  auto x_gen = std::bind(x_distribution, x_generator);
  auto y_gen = std::bind(y_distribution, y_generator);

  for (size_t i = 0; i < seeds.size(); ++i) {
    seeds[i].set_coord(seeds[i].x + x_gen(), seeds[i].y + y_gen());
  }
}
