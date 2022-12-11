#ifndef PIPELINE_IP_MORPHOLOGICAL_PROCESSOR_H
#define PIPELINE_IP_MORPHOLOGICAL_PROCESSOR_H

#include <memory>
#include <string>

#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string MORPH_OPERATION = "Operation";
const std::string MORPH_DILATATION = "Dilatation";
const std::string MORPH_EROSION = "Erosion";
const std::string MORPH_KERNEL_SIZE = "Kernel size";

class MorphologicalProcessor : public BaseProcessor
{
public:
  MorphologicalProcessor();
  ~MorphologicalProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif