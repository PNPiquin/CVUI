#ifndef PIPELINE_IP_CANNY_PROCESSOR_H
#define PIPELINE_IP_CANNY_PROCESSOR_H

#include <string>

#include "pipeline/processor.h"

const std::string CANNY_LOW_THRESHOLD = "Low threshold";
const std::string CANNY_HIGH_THRESHOLD = "High threshold";

class CannyProcessor : public BaseProcessor
{
public:
  CannyProcessor();
  ~CannyProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif