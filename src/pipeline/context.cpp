#include "pipeline/context.h"

#include "image_processing/utils.h"
#include "utils/lodepng.h"

Context::Context() {}

void Context::add_image(std::string img_name, Image img)
{
  // We want an overwrite behavior, so first try to delete img if it already exists
  delete_image(img_name);

  if (img.type == ImageType::GRAY) {
    add_gray_image(img_name, img.gray_img);
  } else if (img.type == ImageType::RGBA) {
    add_rgba_image(img_name, img.rgba_img);
  } else if (img.type == ImageType::FULL) {
    add_gray_image(img_name, img.gray_img);
    add_rgba_image(img_name, img.rgba_img);
  }
}

Image Context::get_image(std::string img_name)
{
  Image img;
  auto rgba_img = get_rgba_image(img_name);
  if (rgba_img->get_cols() != 0) {
    img.rgba_img = rgba_img;
    img.type = ImageType::RGBA;
  }
  auto gray_img = get_gray_image(img_name);
  if (gray_img->get_cols() != 0) {
    img.gray_img = gray_img;
    img.type = ImageType::GRAY;
  }
  if ((rgba_img->get_cols() != 0) & (gray_img->get_cols() != 0)) {
    img.type = ImageType::FULL;
  }
  return img;
}

void Context::delete_image(std::string img_name)
{
  imgs.erase(img_name);
  gray_imgs.erase(img_name);
}

void Context::add_rgba_image(std::string img_name, std::shared_ptr<Matrix<uint32_t>> img)
{
  imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint32_t>> Context::get_rgba_image(std::string img_name)
{
  auto img = imgs.find(img_name);
  if (img == imgs.end()) {
    return std::make_shared<Matrix<uint32_t>>();
  }
  return img->second;
}

void Context::add_gray_image(std::string img_name, std::shared_ptr<Matrix<uint8_t>> img)
{
  gray_imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint8_t>> Context::get_gray_image(std::string img_name, bool convert_color_img)
{
  auto img = gray_imgs.find(img_name);
  if (img == gray_imgs.end()) {
    if (convert_color_img) {
      auto color_img = get_rgba_image(img_name);
      if (color_img->get_rows() == 0) {
        return std::make_shared<Matrix<uint8_t>>();
      } else {
        return ip::rgba_to_gray(color_img);
      }
    } else {
      return std::make_shared<Matrix<uint8_t>>();
    }
  }
  return img->second;
}

void Context::save_gray_image(std::string img_name)
{
  auto gray_img = get_gray_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  save_gray_image(gray_img, img_name + ".png");
}

void Context::save_gray_image(std::shared_ptr<Matrix<uint8_t>> gray_img, std::string filepath)
{
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

  lodepng::encode(filepath, png_data, gray_img->get_cols(), gray_img->get_rows());
}

void Context::save_image(std::string img_name)
{
  auto rgba_img = get_rgba_image(img_name);
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
