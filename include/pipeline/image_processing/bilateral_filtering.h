#ifndef PIPELINE_IP_BILATERAL_FILTERING_H
#define PIPELINE_IP_BILATERAL_FILTERING_H

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

#include "pipeline/configuration.h"
#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string BF_KERNEL_SIZE = "Kernel semi size";
const std::string BF_SIGMA = "Standard deviation";

class BilateralFilteringProcessor : public BaseProcessor
{
public:
  BilateralFilteringProcessor();
  ~BilateralFilteringProcessor() = default;

  virtual bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif