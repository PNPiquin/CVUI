#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdint>
#include <string>
#include <utils/contextbase.h>

class GrayContext : public Context<uint8_t>
{
public:
  void process_kmeans(std::string img_name);
  void process_kmeans(std::string img_name, std::string output_name);

  void generate_random_framing(std::string img_name);
  void generate_random_framing(std::string img_name, std::string output_name);

  void generate_grid_framing(std::string img_name);
  void generate_grid_framing(std::string img_name, std::string output_name);

  void generate_border_image(std::string img_name);
  void generate_border_image(std::string img_name, std::string output_name);

  void save_image(std::string img_name);
};

class RGBAContext : public Context<uint32_t>
{
public:
  void apply_framing(std::string img_name, std::shared_ptr<Matrix<uint8_t>> framing);
  void apply_framing(std::string img_name, std::shared_ptr<Matrix<uint8_t>> framing, std::string output_name);

  void save_image(std::string img_name);
  static void save_image(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath);
};

#endif