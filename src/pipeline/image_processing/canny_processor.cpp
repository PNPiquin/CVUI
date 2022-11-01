#include "pipeline/image_processing/canny_processor.h"

#include "image_processing/canny.h"

CannyProcessor::CannyProcessor()
{
  processor_name = "Canny edge detection";
  processor_suffix = "_canny";

  config.set_double_property(CANNY_LOW_THRESHOLD, 10.);
  config.set_double_property(CANNY_HIGH_THRESHOLD, 30.);
}

bool CannyProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  Image img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  double low_threshold = config.get_double(CANNY_LOW_THRESHOLD);
  double high_threshold = config.get_double(CANNY_HIGH_THRESHOLD);
  CannyEdgeDetector canny_edge_detector(low_threshold, high_threshold);
  if (img.type == ImageType::GRAY || img.type == ImageType::FULL) {
    canny_edge_detector.process_gray_img(img.gray_img);
  } else {
    canny_edge_detector.process_rgba_img(img.rgba_img);
  }
  context.add_image(output_img_name, Image(canny_edge_detector.get_canny_edges()));

  return true;
}
