#include "pipeline/image_processing/morphological_processor.h"

#include "image_processing/morphological_operations.h"

MorphologicalProcessor::MorphologicalProcessor()
{
  processor_name = "Morpological operations";
  processor_suffix = "_morph";

  // Setup configuration
  config.set_integer_property(MORPH_KERNEL_SIZE, 2);

  EnumType morph_operations = EnumType();
  morph_operations.add_value(MORPH_DILATATION);
  morph_operations.add_value(MORPH_EROSION);
  config.set_enum_property(MORPH_OPERATION, morph_operations);
}

bool MorphologicalProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  int kernel_size = config.get_int(MORPH_KERNEL_SIZE);
  std::string operation_name = config.get_enum_value(MORPH_OPERATION);

  if (operation_name == MORPH_DILATATION) {
    if (img.type == ImageType::FULL || img.type == ImageType::RGBA) {
      context.add_image(output_img_name, ip::apply_dilatation(img.rgba_img, kernel_size));
    } else if (img.type == ImageType::GRAY) {
      context.add_image(output_img_name, ip::apply_dilatation(img.gray_img, kernel_size));
    } else {
      return false;
    }
  } else if (operation_name == MORPH_EROSION) {
    if (img.type == ImageType::FULL || img.type == ImageType::RGBA) {
      context.add_image(output_img_name, ip::apply_erosion(img.rgba_img, kernel_size));
    } else if (img.type == ImageType::GRAY) {
      context.add_image(output_img_name, ip::apply_erosion(img.gray_img, kernel_size));
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}
