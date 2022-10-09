#include "pipeline/image_processing/color_processor.h"

#include "image_processing/utils.h"

ColorConversionProcessor::ColorConversionProcessor()
{
  processor_name = "Color conversion processor";
  processor_suffix = "_color";

  EnumType transformations;
  transformations.add_value(COLOR_RGB2HSV);
  transformations.add_value(COLOR_HSV2RGB);
  transformations.add_value(COLOR_RGB2GRAY);
  transformations.add_value(COLOR_GRAY2RGB);
  config.set_enum_property(COLOR_TRANSFORMATION, transformations);
}

bool ColorConversionProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  Image img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  std::string transformation = config.get_enum_value(COLOR_TRANSFORMATION);
  if (transformation == COLOR_RGB2HSV) {
    if (img.type != ImageType::FULL && img.type != ImageType::RGBA) {
      return false;
    }
    auto hsva_img = ip::rgba_to_hsva(img.rgba_img);
    context.add_image(output_img_name, Image(hsva_img));
  } else if (transformation == COLOR_HSV2RGB) {
    if (img.type != ImageType::FULL && img.type != ImageType::RGBA) {
      return false;
    }
    auto rgba_img = ip::hsva_to_rgba(img.rgba_img);
    context.add_image(output_img_name, Image(rgba_img));
  } else if (transformation == COLOR_RGB2GRAY) {
    if (img.type != ImageType::FULL && img.type != ImageType::RGBA) {
      return false;
    }
    auto gray_img = ip::rgba_to_gray(img.rgba_img);
    context.add_image(output_img_name, Image(gray_img));
  } else if (transformation == COLOR_GRAY2RGB) {
    if (img.type != ImageType::GRAY && img.type != ImageType::FULL) {
      return false;
    }
    auto rgba_img = ip::gray_to_rgba(img.gray_img);
    context.add_image(output_img_name, Image(rgba_img));
  } else {
    return false;
  }
  return true;
}

// ------------------------------------------------------------------------------------------------
//                                     HSV Processor
// ------------------------------------------------------------------------------------------------
HSVProcessor::HSVProcessor()
{
  processor_name = "HSV processor";
  processor_suffix = "_hsv";

  config.set_double_property(HSV_HUE_ROTATION, 0.0);
  config.set_double_property(HSV_SATURATION_FACTOR, 1.0);
  config.set_double_property(HSV_VALUE_FACTOR, 1.0);
}

bool HSVProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  Image img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN || img.type == ImageType::GRAY) {
    return false;
  }

  double hue_rotation = config.get_double(HSV_HUE_ROTATION);
  double saturation_factor = config.get_double(HSV_SATURATION_FACTOR);
  double value_factor = config.get_double(HSV_VALUE_FACTOR);
  auto out_img = ip::hsva_transformation(img.rgba_img, hue_rotation, saturation_factor, value_factor);
  context.add_image(output_img_name, Image(out_img));
  return true;
}
