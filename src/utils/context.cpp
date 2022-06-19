#include "utils/context.h"
#include "utils/lodepng.h"

#include "image_processing/kmeans.h"
#include <vector>

void GrayContext::process_kmeans(std::string img_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  // Initialize output image
  auto img_out = std::make_shared<Matrix<uint8_t>>(gray_img->get_rows(), gray_img->get_cols());

  KMeans kmeans(50);
  kmeans.process_kmeans(gray_img, img_out);

  add_image(img_name + "_kmeans", img_out);
}

void GrayContext::save_image(std::string img_name)
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

void RGBAContext::save_image(std::string img_name)
{
  auto rgba_img = get_image(img_name);
  if (!rgba_img) {
    // No image, no-op
    return;
  }

  size_t img_width = rgba_img->get_cols();
  size_t img_height = rgba_img->get_rows();
  std::vector<uint8_t> png_data;
  png_data.resize(rgba_img->get_cols() * rgba_img->get_rows() * 4);
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      uint8_t rgba_pixel = rgba_img->operator()(row, col);
      png_data[(row * img_width + col) * 4] = uint8_t((rgba_pixel & 0xff000000) >> 24);
      png_data[(row * img_width + col) * 4 + 1] = uint8_t((rgba_pixel & 0x00ff0000) >> 16);
      png_data[(row * img_width + col) * 4 + 2] = uint8_t((rgba_pixel & 0x0000ff00) >> 8);
      png_data[(row * img_width + col) * 4 + 3] = uint8_t((rgba_pixel & 0x000000ff));
    }
  }

  lodepng::encode(img_name + ".png", png_data, rgba_img->get_cols(), rgba_img->get_rows());
}
