#ifndef PIPELINE_NORMALIZATION_PROCESSOR_H
#define PIPELINE_NORMALIZATION_PROCESSOR_H

#include "pipeline/configuration.h"
#include "pipeline/context.h"
#include "pipeline/processor.h"

class NormalizationProcessor : public BaseProcessor
{
public:
  NormalizationProcessor();
  ~NormalizationProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif