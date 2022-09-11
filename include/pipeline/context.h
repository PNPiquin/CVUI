#ifndef PIPELINE_CONTEXT_H
#define PIPELINE_CONTEXT_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "utils/matrix.h"

class Context
{
private:
  std::map<std::string, std::shared_ptr<Matrix<uint32_t>>> imgs;
  std::map<std::string, std::shared_ptr<Matrix<uint8_t>>> gray_imgs;

public:
  Context();
  ~Context() = default;

  void add_image(std::string img_name, std::shared_ptr<Matrix<uint32_t>> img);
  std::shared_ptr<Matrix<uint32_t>> get_image(std::string img_name);

  void add_gray_image(std::string img_name, std::shared_ptr<Matrix<uint8_t>> img);
  std::shared_ptr<Matrix<uint8_t>> get_gray_image(std::string img_name, bool convert_color_img = true);

  void save_image(std::string img_name);
  void save_gray_image(std::string img_name);
  static void save_image(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath);
};

#endif