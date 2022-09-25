#include "pipeline/image_processing/normalization_processor.h"

#include "image_processing/normalization.h"

NormalizationProcessor::NormalizationProcessor()
{
  processor_name = "Normalization processor";
  processor_suffix = "_norm";

  EnumType normalization_method_enum = EnumType();
  normalization_method_enum.add_value("Min - Max");
  config.set_enum_property("normalization method", normalization_method_enum);
}

bool NormalizationProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  if (img.type == ImageType::FULL || img.type == ImageType::RGBA) {
    auto res_img = ip::min_max_normalization(img.rgba_img);
    context.add_image(output_img_name, Image(res_img));
  } else if (img.type == ImageType::GRAY) {
    auto res_img = ip::min_max_normalization(img.gray_img);
    context.add_image(output_img_name, Image(res_img));
  } else {
    return false;
  }

  return true;
}
