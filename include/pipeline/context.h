#ifndef PIPELINE_CONTEXT_H
#define PIPELINE_CONTEXT_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "utils/matrix.h"

enum ImageType
{
  UNKNOWN,
  GRAY,
  RGBA,
  FULL
};

struct Image
{
  ImageType type = ImageType::UNKNOWN;

  // Placeholder for RGBA and Gray images
  std::shared_ptr<Matrix<uint32_t>> rgba_img = nullptr;
  std::shared_ptr<Matrix<uint8_t>> gray_img = nullptr;

  Image(){};
  Image(std::shared_ptr<Matrix<uint8_t>> _gray_img)
  {
    gray_img = _gray_img;
    type = ImageType::GRAY;
  };
  Image(std::shared_ptr<Matrix<uint32_t>> _rgba_img)
  {
    rgba_img = _rgba_img;
    type = ImageType::RGBA;
  };
};

class Context
{
public:
  Context();
  ~Context() = default;

  void add_image(std::string img_name, Image img);
  Image get_image(std::string img_name);
  void delete_image(std::string img_name);

  void add_rgba_image(std::string img_name, std::shared_ptr<Matrix<uint32_t>> img);
  std::shared_ptr<Matrix<uint32_t>> get_rgba_image(std::string img_name);

  void add_gray_image(std::string img_name, std::shared_ptr<Matrix<uint8_t>> img);
  std::shared_ptr<Matrix<uint8_t>> get_gray_image(std::string img_name, bool convert_color_img = true);

  void save_image(std::string img_name);
  void save_gray_image(std::string img_name);
  static void save_image(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath);
  static void save_gray_image(std::shared_ptr<Matrix<uint8_t>> gray_img, std::string filepath);

private:
  std::map<std::string, std::shared_ptr<Matrix<uint32_t>>> imgs;
  std::map<std::string, std::shared_ptr<Matrix<uint8_t>>> gray_imgs;
};

#endif