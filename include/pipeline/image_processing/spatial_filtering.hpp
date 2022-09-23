#ifndef PIPELINE_IP_SPATIAL_FILTERING_HPP
#define PIPELINE_IP_SPATIAL_FILTERING_HPP

#define M_PI 3.14159265358979323846

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

#include "pipeline/configuration.h"
#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string KERNEL_SIZE = "Kernel size";

class GaussianBlurProcessor : public BaseProcessor
{
public:
  GaussianBlurProcessor();
  ~GaussianBlurProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;

private:
  std::shared_ptr<Matrix<float>> create_kernel(int kernel_size, float sigma = 1.f);
};

class EdgeDetectionProcessor : public BaseProcessor
{
public:
  EdgeDetectionProcessor();
  ~EdgeDetectionProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;

private:
  std::shared_ptr<Matrix<float>> create_kernel(int kernel_size);
};

#endif