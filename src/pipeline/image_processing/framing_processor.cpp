#include "pipeline/image_processing/framing_processor.h"

#include "image_processing/framing.h"

FramingProcessor::FramingProcessor()
{
  processor_name = "Framing processor";
  processor_suffix = "_framing";

  // Setup the configuration
  // Define the starting grid
  config.set_integer_property(FRAMING_ROWS, 10);
  config.set_integer_property(FRAMING_COLS, 10);

  config.set_boolean_property(FRAMING_RANDOMIZE, true);
  config.set_integer_property(FRAMING_ROW_TOLERANCE, 10);
  config.set_integer_property(FRAMING_COL_TOLERANCE, 10);
}

bool FramingProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  FramingConfiguration framing_config{
    config.get_int(FRAMING_ROWS),          config.get_int(FRAMING_COLS),          config.get_bool(FRAMING_RANDOMIZE),
    config.get_int(FRAMING_ROW_TOLERANCE), config.get_int(FRAMING_COL_TOLERANCE),
  };
  FramingService framing_service(framing_config);
  Image img = context.get_image(img_name);

  // If image is null, return false
  if (img.type != ImageType::GRAY) {
    return false;
  }

  auto out_img = framing_service.create_zones(img.gray_img);
  context.add_image(output_img_name, Image(out_img));

  return true;
}

bool FramingProcessor::apply_framing(Context context,
                                     std::string base_img_name,
                                     std::string framing_img_name,
                                     std::string output_img_name)
{
  auto img = context.get_image(base_img_name);
  auto framing_img = context.get_image(framing_img_name);

  // If image is null, return false
  if (img.type != ImageType::FULL && img.type != ImageType::RGBA) {
    return false;
  }
  if (framing_img.type != ImageType::GRAY) {
    return false;
  }

  // Initialize output image
  auto img_out = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());

  size_t img_width = img.rgba_img->get_cols();
  size_t img_height = img.rgba_img->get_rows();
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      if (framing_img.gray_img->operator()(row, col) != 0) {
        img_out->operator()(row, col) = 0;
      } else {
        img_out->operator()(row, col) = img.rgba_img->operator()(row, col);
      }
    }
  }

  context.add_image(output_img_name, Image(img_out));
  return true;
}
