#ifndef PIPELINE_FRAMING_PROCESSOR_H
#define PIPELINE_FRAMING_PROCESSOR_H

#include <string>

#include "pipeline/processor.h"

const std::string FRAMING_ROWS = "Number of seed rows";
const std::string FRAMING_COLS = "Number of seed cols";
const std::string FRAMING_RANDOMIZE = "Randomize seeds";
const std::string FRAMING_ROW_TOLERANCE = "Seed row tolerance";
const std::string FRAMING_COL_TOLERANCE = "Seed col tolerance";

class FramingProcessor : public BaseProcessor
{
public:
  FramingProcessor();
  ~FramingProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
  bool apply_framing(Context context,
                     std::string base_img_name,
                     std::string framing_img_name,
                     std::string output_img_name);
};

#endif