#ifndef PIPELINE_IP_COMPLEX_CRESTLINE_DETECTOR_H
#define PIPELINE_IP_COMPLEX_CRESTLINE_DETECTOR_H

#include <cstdint>
#include <string>

#include "pipeline/processor.h"

const std::string CD_KERNEL_SIZE = "Kernel size";
const std::string CD_KERNEL_STD = "Kernel standard deviation";

const std::string CD_CANNY_LOW_THRESHOLD = "Low threshold";
const std::string CD_CANNY_HIGH_THRESHOLD = "High threshold";

const std::string CD_DILATATION_SIZE = "Dilatation kernel size";
const std::string CD_DILATATION_REPETITTION = "Number of dilatation";

const std::string CD_X_MIN_SPAN = "Minimun vertical span";
const std::string CD_Y_MIN_SPAN = "Minimun horizontal span";
const std::string CD_XY_MIN_RATIO = "Minimun horizontal / vertical ratio";
const std::string CD_XY_MAX_RATIO = "Maximum horizontal / vertical ratio";

const std::string CD_KEEP_TOP_PIXELS = "Width of top line to keep";

class CrestlineDetector : public BaseProcessor
{
public:
  CrestlineDetector();
  ~CrestlineDetector() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;

private:
  std::shared_ptr<Matrix<float>> create_kernel(Configuration& config);

  std::shared_ptr<Matrix<uint8_t>> select_top_pixels(std::shared_ptr<Matrix<uint8_t>> input_img, int n_pixels);
  std::shared_ptr<Matrix<uint32_t>> create_texture_img(std::shared_ptr<Matrix<uint8_t>> crestine_img,
                                                       std::shared_ptr<Matrix<uint32_t>> laplacian_img);
};

#endif