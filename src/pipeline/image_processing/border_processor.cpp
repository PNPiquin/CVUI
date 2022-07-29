#include "pipeline/image_processing/border_processor.h"

#include "image_processing/zone_utils.h"

BorderProcessor::BorderProcessor()
{
  // Setup the configuration
  // Define the starting grid
  config.set_integer_property(BORDER_THRESHOLD, 0);
  config.set_integer_property(BORDER_NEIGHBORHOOD_SIZE, 2);
}

bool BorderProcessor::process(Context context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_gray_image(img_name);

  // If image is null, return false
  if (img->get_rows() == 0) {
    return false;
  }

  auto out_img = ip::get_borders(img, config.get_int(BORDER_THRESHOLD), config.get_int(BORDER_NEIGHBORHOOD_SIZE));
  context.add_gray_image(output_img_name, out_img);

  return true;
}
