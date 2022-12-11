#include "pipeline/image_processing/spatial_filtering.h"

#include "image_processing/morphological_operations.h"
#include "image_processing/spatial_filtering.h"

// ------------------------------------------------------------------------------------------------
//                                     SPATIAL FILTERING BASE
// ------------------------------------------------------------------------------------------------
SpatialFilteringProcessor::SpatialFilteringProcessor()
{
  processor_name = "Spatial filtering processor";
  processor_suffix = "_sp_base";
}

bool SpatialFilteringProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  // Apply kernel
  auto kernel = create_kernel(config);
  if (img.type == ImageType::RGBA || img.type == ImageType::FULL) {
    auto res_img = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
    ip::apply_kernel(img.rgba_img, kernel, res_img);
    context.add_image(output_img_name, Image(res_img));
  } else {
    auto res_img = std::make_shared<Matrix<uint8_t>>(img.gray_img->get_rows(), img.gray_img->get_cols());
    ip::apply_kernel(img.gray_img, kernel, res_img);
    context.add_image(output_img_name, Image(res_img));
  }
  return true;
}

// ------------------------------------------------------------------------------------------------
//                                     GAUSSIAN BLUR
// ------------------------------------------------------------------------------------------------
GaussianBlurProcessor::GaussianBlurProcessor()
{
  processor_name = "Gaussian blur processor";
  processor_suffix = "_gaussian_blur";

  // Setup configuration
  config.set_integer_property(KERNEL_SIZE, 5);
  config.set_double_property(KERNEL_STD, 1.0);
}

std::shared_ptr<Matrix<float>> GaussianBlurProcessor::create_kernel(Configuration& config)
{
  int kernel_size = config.get_int(KERNEL_SIZE);
  double sigma = config.get_double(KERNEL_STD);
  return ip::create_gaussian_kernel(kernel_size, sigma);
}

// ------------------------------------------------------------------------------------------------
//                                     EDGE DETECTION
// ------------------------------------------------------------------------------------------------
EdgeDetectionProcessor::EdgeDetectionProcessor()
{
  processor_name = "Edge detection processor";
  processor_suffix = "_edge";

  // Setup configuration
  config.set_integer_property(KERNEL_SIZE, 5);
  config.set_double_property(KERNEL_STD, 1.0);

  EnumType edge_detection_algorithms = EnumType();
  edge_detection_algorithms.add_value(EDGE_KERNEL_ALGORITHM);
  edge_detection_algorithms.add_value(EDGE_MORPH_H_ALGORITHM);
  edge_detection_algorithms.add_value(EDGE_MORPH_V_ALGORITHM);
  config.set_enum_property(EDGE_ALGORITHM, edge_detection_algorithms);
}

std::shared_ptr<Matrix<float>> EdgeDetectionProcessor::create_kernel(Configuration& config)
{
  // kernel size can either be 3 or 5. If input is not 3, we create a 5x5 kernel
  size_t kernel_size = size_t(config.get_int(KERNEL_SIZE));
  auto kernel = std::make_shared<Matrix<float>>(kernel_size, kernel_size);

  if (kernel_size == 3) {
    for (size_t x = 0; x < kernel_size; ++x) {
      for (size_t y = 0; y < kernel_size; ++y) {
        kernel->operator()(x, y) = -0.125f;
      }
    }
    kernel->operator()(1, 1) = 1.f;
  } else if (kernel_size == 5) {
    for (size_t x = 0; x < kernel_size; ++x) {
      for (size_t y = 0; y < kernel_size; ++y) {
        kernel->operator()(x, y) = -1.f / 32.f;
      }
    }
    for (size_t x = 1; x < kernel_size - 1; ++x) {
      for (size_t y = 1; y < kernel_size - 1; ++y) {
        kernel->operator()(x, y) = -2.f / 32.f;
      }
    }
    kernel->operator()(2, 2) = 1.f;
  } else {
    size_t kernel_semi_size = kernel_size / 2;
    float sigma = float(config.get_double(KERNEL_STD));
    float alpha = -1.f / (4.f * M_PI * std::pow(sigma, 4.f));
    for (size_t x = 0; x < kernel_size; ++x) {
      for (size_t y = 0; y < kernel_size; ++y) {
        float x2_y2 = std::pow(float(kernel_semi_size) - x, 2.f) + std::pow(float(kernel_semi_size) - y, 2.f);
        float x2_y2_norm = x2_y2 / (2.f * std::pow(sigma, 2.f));
        kernel->operator()(x, y) = alpha * (1.f - x2_y2_norm) * std::exp(-x2_y2_norm);
      }
    }
  }
  return kernel;
}

bool EdgeDetectionProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  // Apply kernel
  auto kernel = create_kernel(config);
  int kernel_size = config.get_int(KERNEL_SIZE);
  std::string algorithm = config.get_enum_value(EDGE_ALGORITHM);

  if (img.type == ImageType::RGBA || img.type == ImageType::FULL) {
    auto res_img = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
    if (algorithm == EDGE_MORPH_H_ALGORITHM) {
      res_img = ip::apply_h_gradient(img.rgba_img, kernel_size);
    } else if (algorithm == EDGE_MORPH_V_ALGORITHM) {
      res_img = ip::apply_v_gradient(img.rgba_img, kernel_size);
    } else {
      ip::apply_kernel(img.rgba_img, kernel, res_img);
    }
    context.add_image(output_img_name, Image(res_img));
  } else {
    auto res_img = std::make_shared<Matrix<uint8_t>>(img.gray_img->get_rows(), img.gray_img->get_cols());
    if (algorithm == EDGE_MORPH_H_ALGORITHM) {
      res_img = ip::apply_h_gradient(img.gray_img, kernel_size);
    } else if (algorithm == EDGE_MORPH_V_ALGORITHM) {
      res_img = ip::apply_v_gradient(img.gray_img, kernel_size);
    } else {
      ip::apply_kernel(img.gray_img, kernel, res_img);
    }
    context.add_image(output_img_name, Image(res_img));
  }
  return true;
}
