#include "utils/context.h"
#include "utils/lodepng.h"

#include "image_processing/border.h"
#include "image_processing/framing.h"
#include "image_processing/kmeans.h"
#include "image_processing/segmentation.h"
#include <vector>

// --------------------------------------------------------------------------------------------------------------------
//                                    GRAY CONTEXT
// --------------------------------------------------------------------------------------------------------------------
void GrayContext::process_kmeans(std::string img_name)
{
  process_kmeans(img_name, img_name + "_kmeans");
}

void GrayContext::process_kmeans(std::string img_name, std::string output_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  // Initialize output image
  auto img_out = std::make_shared<Matrix<uint8_t>>(gray_img->get_rows(), gray_img->get_cols());

  KMeans kmeans(75);
  kmeans.process_kmeans(gray_img, img_out);

  add_image(output_name, img_out);
}

void GrayContext::generate_random_framing(std::string img_name)
{
  generate_random_framing(img_name, img_name + "_random_framing");
}

void GrayContext::generate_random_framing(std::string img_name, std::string output_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  auto img_out = ip::get_random_framing(gray_img, 120);

  add_image(output_name, img_out);
}

void GrayContext::generate_grid_framing(std::string img_name)
{
  generate_grid_framing(img_name, img_name + "_grid_framing");
}

void GrayContext::generate_grid_framing(std::string img_name, std::string output_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  FramingConfiguration config = { 10, 12, true, 90, 75 };
  FramingService framing_service(config);
  auto img_out = framing_service.create_zones(gray_img);

  add_image(output_name, img_out);
}

void GrayContext::generate_border_image(std::string img_name)
{
  generate_border_image(img_name, img_name + "_border");
}

void GrayContext::generate_border_image(std::string img_name, std::string output_name)
{
  auto gray_img = get_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  auto border_img = ip::get_borders(gray_img, 0, 2);
  add_image(output_name, border_img);
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

// --------------------------------------------------------------------------------------------------------------------
//                                    RGBA CONTEXT
// --------------------------------------------------------------------------------------------------------------------
void RGBAContext::apply_framing(std::string img_name, std::shared_ptr<Matrix<uint8_t>> framing)
{
  apply_framing(img_name, framing, img_name + "_framing");
}

void RGBAContext::apply_framing(std::string img_name, std::shared_ptr<Matrix<uint8_t>> framing, std::string output_name)
{
  auto rgba_img = get_image(img_name);
  if (!rgba_img) {
    // No image, no-op
    return;
  }

  // Initialize output image
  auto img_out = std::make_shared<Matrix<uint32_t>>(rgba_img->get_rows(), rgba_img->get_cols());

  size_t img_width = rgba_img->get_cols();
  size_t img_height = rgba_img->get_rows();
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      if (framing->operator()(row, col) != 0) {
        img_out->operator()(row, col) = 0;
      } else {
        img_out->operator()(row, col) = rgba_img->operator()(row, col);
      }
    }
  }
  add_image(output_name, img_out);
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
      uint32_t rgba_pixel = rgba_img->operator()(row, col);
      png_data[(row * img_width + col) * 4] = uint8_t((rgba_pixel & 0xff000000) >> 24);
      png_data[(row * img_width + col) * 4 + 1] = uint8_t((rgba_pixel & 0x00ff0000) >> 16);
      png_data[(row * img_width + col) * 4 + 2] = uint8_t((rgba_pixel & 0x0000ff00) >> 8);
      png_data[(row * img_width + col) * 4 + 3] = uint8_t((rgba_pixel & 0x000000ff));
    }
  }

  lodepng::encode(img_name + ".png", png_data, rgba_img->get_cols(), rgba_img->get_rows());
}