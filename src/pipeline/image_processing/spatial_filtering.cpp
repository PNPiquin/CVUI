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
  auto img = context.get_gray_image(img_name);

  // If image is null, return false
  if (img->get_rows() == 0) {
    return false;
  }

  // Apply gaussian kernel
  int kernel_size = config.get_int(KERNEL_SIZE);
  auto kernel = create_kernel(kernel_size);
  auto res_img = std::make_shared<Matrix<uint8_t>>(img->get_rows(), img->get_cols());

  ip::apply_kernel(img, kernel, res_img);
  context.add_gray_image(img_name + processor_suffix + "_" + std::to_string(kernel_size), res_img);

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
  auto img = context.get_gray_image(img_name);

  // If image is null, return false
  if (img->get_rows() == 0) {
    return false;
  }

  // Apply gaussian kernel
  int kernel_size = config.get_int(KERNEL_SIZE);
  auto kernel = create_kernel(kernel_size);
  auto res_img = std::make_shared<Matrix<uint8_t>>(img->get_rows(), img->get_cols());

  ip::apply_kernel(img, kernel, res_img);
  context.add_gray_image(img_name + processor_suffix + "_" + std::to_string(kernel_size), res_img);

  return true;
}

std::shared_ptr<Matrix<float>> EdgeDetectionProcessor::create_kernel(int kernel_size)
{
  auto kernel = std::make_shared<Matrix<float>>(kernel_size, kernel_size);
  float kernel_squared = float(kernel_size * kernel_size);
  int kernel_semi_size = (kernel_size - 1) / 2;
  for (size_t row = 0; row < size_t(kernel_size); ++row) {
    for (size_t col = 0; col < size_t(kernel_size); ++col) {
      kernel->operator()(row, col) = -1. / kernel_squared;
    }
  }
  kernel->operator()(kernel_semi_size, kernel_semi_size) = (kernel_squared - 1.) / kernel_squared;
  return kernel;
}
