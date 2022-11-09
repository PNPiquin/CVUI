#ifndef PIPELINE_IP_RESIZING_PROCESSOR_H
#define PIPELINE_IP_RESIZING_PROCESSOR_H

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

#include "pipeline/configuration.h"
#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string RP_TARGET_ROWS = "Target rows";
const std::string RP_TARGET_COLS = "Target cols";
const std::string RP_INTERPOLATION_TYPE = "Interpolation type";
const std::string RP_INTERPOLATION_BILINEAR = "Bilinear";

class ResizingProcessor : public BaseProcessor
{
public:
  ResizingProcessor();
  ~ResizingProcessor() = default;

  virtual bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif