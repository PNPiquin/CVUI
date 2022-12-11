#ifndef PIPELINE_IP_BLOB_EXTRACTOR_H
#define PIPELINE_IP_BLOB_EXTRACTOR_H

#include <string>

#include "pipeline/processor.h"

const std::string BE_X_MIN_SPAN = "Minimun vertical span";
const std::string BE_Y_MIN_SPAN = "Minimun horizontal span";
const std::string BE_XY_MIN_RATIO = "Minimun horizontal / vertival ratio";
const std::string BE_XY_MAX_RATIO = "Maximum horizontal / vertical ratio";

class BlobExtractor : public BaseProcessor
{
public:
  BlobExtractor();
  ~BlobExtractor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif