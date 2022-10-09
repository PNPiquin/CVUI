#ifndef PIPELINE_IP_COLOR_PROCESSOR_H
#define PIPELINE_IP_COLOR_PROCESSOR_H

#include <cstdint>
#include <memory>
#include <string>

#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string COLOR_TRANSFORMATION = "Transformation";
const std::string COLOR_RGB2HSV = "RGB to HSV";
const std::string COLOR_HSV2RGB = "HSV to RGB";
const std::string COLOR_RGB2GRAY = "RGB to Gray";
const std::string COLOR_GRAY2RGB = "Gray to RGB";

class ColorConversionProcessor : public BaseProcessor
{
public:
  ColorConversionProcessor();
  ~ColorConversionProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

const std::string HSV_HUE_ROTATION = "Hue rotation";
const std::string HSV_SATURATION_FACTOR = "Saturation factor";
const std::string HSV_VALUE_FACTOR = "Value factor";

class HSVProcessor : public BaseProcessor
{
public:
  HSVProcessor();
  ~HSVProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif