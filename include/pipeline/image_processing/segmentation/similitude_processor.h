#ifndef PIPELINE_IP_SIMILITUDE_PROCESSOR_H
#define PIPELINE_IP_SIMILITUDE_PROCESSOR_H

#include <string>

#include "pipeline/processor.h"

const std::string SIMILITUDE_THRESHOLD = "Similitude threshold";
const std::string SIMILITUDE_REGION_WIDTH = "Region width";
const std::string SIMILITUDE_REGION_HEIGHT = "Region height";
const std::string SIMILITUDE_MIN_REGION_SIZE = "Minimum region size";
const std::string SIMILITUDE_MERGE_REGIONS = "Merge regions";

class SimilitudeProcessor : public BaseProcessor
{
public:
  SimilitudeProcessor();
  ~SimilitudeProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif