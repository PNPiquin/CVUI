#include "pipeline/image_processing/bilateral_filtering.h"

#include "image_processing/bilateral_filtering.h"

BilateralFilteringProcessor::BilateralFilteringProcessor()
{
  processor_name = "Bilateral filtering";
  processor_suffix = "_bilateral";

  config.set_integer_property(BF_KERNEL_SIZE, 3);
  config.set_double_property(BF_SIGMA, 1.);
}

bool BilateralFilteringProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  Image img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN || img.type == ImageType::GRAY) {
    return false;
  }

  double kernel_size = config.get_int(BF_KERNEL_SIZE);
  double sigma = config.get_double(BF_SIGMA);

  BilateralFilter bilateral_filter(kernel_size, 0.f, float(sigma));
  auto out_img = bilateral_filter.apply_on_img(img.rgba_img);
  context.add_image(output_img_name, Image(out_img));
  return true;
}