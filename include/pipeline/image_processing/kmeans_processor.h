#ifndef PIPELINE_IMAGE_PROCESSING_KMEANS_H
#define PIPELINE_IMAGE_PROCESSING_KMEANS_H

#include <memory>

#include "pipeline/processor.h"
#include "utils/matrix.h"

const std::string KMEANS_N_CLUSTER = "Number of cluster";
const std::string KMEANS_DISTANCE_SELECTION = "Distance method";
const std::string KMEANS_MAX_STEPS = "Max computation steps";

class KMeansProcessor : public BaseProcessor
{
public:
  KMeansProcessor();
  ~KMeansProcessor() = default;

  bool process(Context& context, std::string img_name, std::string output_img_name) override;
};

#endif