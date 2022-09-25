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
const std::string KERNEL_STD = "Kernel standard deviation";

class SpatialFilteringProcessor : public BaseProcessor
{
public:
  SpatialFilteringProcessor();
  ~SpatialFilteringProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;

private:
  virtual std::shared_ptr<Matrix<float>> create_kernel(Configuration& config)
  {
    return std::make_shared<Matrix<float>>();
  };
};

class GaussianBlurProcessor : public SpatialFilteringProcessor
{
public:
  GaussianBlurProcessor();
  ~GaussianBlurProcessor() = default;

private:
  std::shared_ptr<Matrix<float>> create_kernel(Configuration& config) override;
};

class EdgeDetectionProcessor : public SpatialFilteringProcessor
{
public:
  EdgeDetectionProcessor();
  ~EdgeDetectionProcessor() = default;

private:
  std::shared_ptr<Matrix<float>> create_kernel(Configuration& config) override;
};

#endif