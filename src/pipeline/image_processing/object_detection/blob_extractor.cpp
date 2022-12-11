#include "pipeline/image_processing/object_detection/blob_extractor.h"

#include "image_processing/object_detection/blob_detection.h"
#include "image_processing/utils.h"
#include <cstdint>

BlobExtractor::BlobExtractor()
{
  processor_name = "Blob extractor";
  processor_suffix = "_blob";

  config.set_double_property(BE_X_MIN_SPAN, 0.);
  config.set_double_property(BE_Y_MIN_SPAN, 0.);
  config.set_double_property(BE_XY_MIN_RATIO, 0.);
  config.set_double_property(BE_XY_MAX_RATIO, 1e5);
}

bool BlobExtractor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN) {
    return false;
  }

  std::shared_ptr<Matrix<uint8_t>> input_img;
  if (img.type == ImageType::RGBA) {
    input_img = ip::rgba_to_gray(img.rgba_img);
  } else {
    input_img = img.gray_img;
  }

  // Extract configuration
  float x_min_span = config.get_double(BE_X_MIN_SPAN);
  float y_min_span = config.get_double(BE_Y_MIN_SPAN);
  float xy_min_ratio = config.get_double(BE_XY_MIN_RATIO);
  float xy_max_ratio = config.get_double(BE_XY_MAX_RATIO);

  BlobDetector blob_detector(x_min_span, y_min_span, xy_min_ratio, xy_max_ratio);
  auto res_img = blob_detector.process(input_img);
  context.add_image(output_img_name, Image(res_img));
  return true;
}