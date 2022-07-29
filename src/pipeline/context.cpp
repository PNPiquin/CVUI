#include "pipeline/context.h"

#include "utils/lodepng.h"

Context::Context() {}

void Context::add_image(std::string img_name, std::shared_ptr<Matrix<uint32_t>> img)
{
  imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint32_t>> Context::get_image(std::string img_name)
{
  auto img = imgs.find(img_name);
  if (img == imgs.end()) {
    return std::shared_ptr<Matrix<uint32_t>>();
  }
  return img->second;
}

void Context::add_gray_image(std::string img_name, std::shared_ptr<Matrix<uint8_t>> img)
{
  gray_imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint8_t>> Context::get_gray_image(std::string img_name)
{
  auto img = gray_imgs.find(img_name);
  if (img == gray_imgs.end()) {
    return std::shared_ptr<Matrix<uint8_t>>();
  }
  return img->second;
}

void Context::save_gray_image(std::string img_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  size_t img_width = gray_img->get_cols();
  size_t img_height = gray_img->get_rows();
  std::vector<uint8_t> png_data;
  png_data.resize(gray_img->get_cols() * gray_img->get_rows() * 4);
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      uint8_t pixel = gray_img->operator()(row, col);
      png_data[(row * img_width + col) * 4] = pixel;
      png_data[(row * img_width + col) * 4 + 1] = pixel;
      png_data[(row * img_width + col) * 4 + 2] = pixel;
      png_data[(row * img_width + col) * 4 + 3] = 255;
    }
  }

  lodepng::encode(img_name + ".png", png_data, gray_img->get_cols(), gray_img->get_rows());
}

void Context::save_image(std::string img_name)
{
  auto rgba_img = get_image(img_name);
  if (!rgba_img) {
    // No image, no-op
    return;
  }

  save_image(rgba_img, img_name + ".png");
}

void Context::save_image(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath)
{
  size_t img_width = rgba_img->get_cols();
  size_t img_height = rgba_img->get_rows();
  std::vector<uint8_t> png_data;
  png_data.resize(rgba_img->get_cols() * rgba_img->get_rows() * 4);
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      uint32_t rgba_pixel = rgba_img->operator()(row, col);
      png_data[(row * img_width + col) * 4] = uint8_t((rgba_pixel & 0xff000000) >> 24);
      png_data[(row * img_width + col) * 4 + 1] = uint8_t((rgba_pixel & 0x00ff0000) >> 16);
      png_data[(row * img_width + col) * 4 + 2] = uint8_t((rgba_pixel & 0x0000ff00) >> 8);
      png_data[(row * img_width + col) * 4 + 3] = uint8_t((rgba_pixel & 0x000000ff));
    }
  }

  lodepng::encode(filepath, png_data, rgba_img->get_cols(), rgba_img->get_rows());
}
