#include "pipeline/image_processing/resizing_processor.h"

#include "image_processing/interpolation.h"

ResizingProcessor::ResizingProcessor()
{
  processor_name = "Resizing processor";
  processor_suffix = "_resizing";

  config.set_integer_property(RP_TARGET_ROWS, 1080);
  config.set_integer_property(RP_TARGET_COLS, 1920);
  EnumType interpolation_type;
  interpolation_type.add_value(RP_INTERPOLATION_BILINEAR);
  config.set_enum_property(RP_INTERPOLATION_TYPE, interpolation_type);
}

bool ResizingProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  Image img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  int target_rows = config.get_int(RP_TARGET_ROWS);
  int target_cols = config.get_int(RP_TARGET_COLS);
  std::string interpolation_type_raw = config.get_enum_value(RP_INTERPOLATION_TYPE);
  ip::INTERPOLATION_TYPE interpolation_type;
  if (interpolation_type_raw == RP_INTERPOLATION_BILINEAR) {
    interpolation_type = ip::INTERPOLATION_TYPE::BILINEAR;
  } else {
    interpolation_type = ip::INTERPOLATION_TYPE::BILINEAR;
  }

  if (img.type == ImageType::GRAY) {
    context.add_image(output_img_name, Image(ip::resize(img.gray_img, target_rows, target_cols, interpolation_type)));
  } else {
    context.add_image(output_img_name, Image(ip::resize(img.rgba_img, target_rows, target_cols, interpolation_type)));
  }
  return true;
}