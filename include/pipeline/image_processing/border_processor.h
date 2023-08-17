#ifndef PIPELINE_BORDER_PROCESSOR_H
#define PIPELINE_BORDER_PROCESSOR_H

#include <string>

#include "pipeline/processor.h"

const std::string BORDER_THRESHOLD = "Threshold to consider while creating zones";
const std::string BORDER_NEIGHBORHOOD_SIZE = "Neighborhood to consider while creating zones";

class BorderProcessor : public BaseProcessor
{
public:
  BorderProcessor();
  ~BorderProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name);
};

#endif