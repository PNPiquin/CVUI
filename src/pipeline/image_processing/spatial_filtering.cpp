#include "pipeline/image_processing/spatial_filtering.hpp"

#include "image_processing/spatial_filtering.hpp"

// ------------------------------------------------------------------------------------------------
//                                     GAUSSIAN BLUR
// ------------------------------------------------------------------------------------------------
GaussianBlurProcessor::GaussianBlurProcessor()
{
  processor_name = "Gaussian blur processor";
  processor_suffix = "_gaussian_blur";

  // Setup configuration
  config.set_integer_property(KERNEL_SIZE, 5);
}

bool GaussianBlurProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  // Apply gaussian kernel
  int kernel_size = config.get_int(KERNEL_SIZE);
  auto kernel = create_kernel(kernel_size);
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

std::shared_ptr<Matrix<float>> GaussianBlurProcessor::create_kernel(int kernel_size, float sigma)
{
  auto kernel = std::make_shared<Matrix<float>>(kernel_size, kernel_size);
  int kernel_semi_size = (kernel_size - 1) / 2;
  for (size_t row = 0; row < size_t(kernel_size); ++row) {
    for (size_t col = 0; col < size_t(kernel_size); ++col) {
      float _row_value = std::pow(float(row) - kernel_semi_size, 2.f);
      float _col_value = std::pow(float(col) - kernel_semi_size, 2.f);
      kernel->operator()(row, col) =
        (1.f / (2.f * M_PI * sigma)) * std::exp(-(_row_value + _col_value) / (2.f * sigma * sigma));
    }
  }
  return kernel;
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
}

bool EdgeDetectionProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  // Apply kernel
  int kernel_size = config.get_int(KERNEL_SIZE);
  auto kernel = create_kernel(kernel_size);
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

std::shared_ptr<Matrix<float>> EdgeDetectionProcessor::create_kernel(int kernel_size)
{
  // kernel size can either be 3 or 5. If input is not 3, we create a 5x5 kernel
  size_t final_kernel_size = kernel_size == 3 ? 3 : 5;
  auto kernel = std::make_shared<Matrix<float>>(final_kernel_size, final_kernel_size);

  if (final_kernel_size == 3) {
    for (size_t x = 0; x < final_kernel_size; ++x) {
      for (size_t y = 0; y < final_kernel_size; ++y) {
        kernel->operator()(x, y) = -0.125f;
      }
    }
    kernel->operator()(1, 1) = 1.f;
  } else {
    for (size_t x = 0; x < final_kernel_size; ++x) {
      for (size_t y = 0; y < final_kernel_size; ++y) {
        kernel->operator()(x, y) = -1.f / 32.f;
      }
    }
    for (size_t x = 1; x < final_kernel_size - 1; ++x) {
      for (size_t y = 1; y < final_kernel_size - 1; ++y) {
        kernel->operator()(x, y) = -2.f / 32.f;
      }
    }
    kernel->operator()(2, 2) = 1.f;
  }
  return kernel;
}
