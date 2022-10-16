#include "pipeline/image_processing/segmentation/similitude_processor.h"

#include "image_processing/segmentation/similitude.h"

SimilitudeProcessor::SimilitudeProcessor()
{
  processor_name = "Similitude processor";
  processor_suffix = "_similitude";

  config.set_integer_property(SIMILITUDE_MIN_REGION_SIZE, 5);
  config.set_integer_property(SIMILITUDE_REGION_HEIGHT, 25);
  config.set_integer_property(SIMILITUDE_REGION_WIDTH, 25);
  config.set_double_property(SIMILITUDE_THRESHOLD, 0.1);
}

bool SimilitudeProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN || img.type == ImageType::GRAY) {
    return false;
  }

  float similitude_threshold = config.get_double(SIMILITUDE_THRESHOLD);
  int region_width = config.get_int(SIMILITUDE_REGION_WIDTH);
  int region_height = config.get_int(SIMILITUDE_REGION_HEIGHT);
  int min_region_size = config.get_int(SIMILITUDE_MIN_REGION_SIZE);
  RegionSimilitude rsim(similitude_threshold, region_width, region_height, min_region_size, img.rgba_img);
  auto res_img = rsim.process();
  context.add_image(output_img_name, Image(res_img));
  return true;
}
