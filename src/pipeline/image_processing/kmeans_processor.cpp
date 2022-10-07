#include "pipeline/image_processing/kmeans_processor.h"

#include "image_processing/kmeans.h"
#include "image_processing/utils.h"

KMeansProcessor::KMeansProcessor()
{
  processor_name = "KMeans processor";
  processor_suffix = "_kmeans";

  // Configuration
  config.set_integer_property(KMEANS_N_CLUSTER, 2);
  config.set_integer_property(KMEANS_MAX_STEPS, 10);

  EnumType distance_enum;
  for (const auto& pair : K_MEANS_DISTANCE_NAMES) {
    distance_enum.add_value(pair.second);
  }
  config.set_enum_property(KMEANS_DISTANCE_SELECTION, distance_enum);
}

bool KMeansProcessor::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);

  // If image is null, return false
  if (img.type == ImageType::UNKNOWN || img.type == ImageType::GRAY) {
    return false;
  }

  // Extract configuration
  int n_clusters = config.get_int(KMEANS_N_CLUSTER);
  int max_steps = config.get_int(KMEANS_MAX_STEPS);
  std::string distance_name = config.get_enum_value(KMEANS_DISTANCE_SELECTION);
  auto it = K_MEANS_DISTANCE_NAMES_TO_ENUM.find(distance_name);
  if (it == K_MEANS_DISTANCE_NAMES_TO_ENUM.end()) {
    return false;
  }
  K_MEANS_DISTANCE distance = it->second;

  KMeans kmeans(n_clusters, distance, max_steps);

  auto final_img = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
  // If we are working on a HSV distance, convert the image
  if (distance == K_MEANS_DISTANCE::HSV_SVD || distance == K_MEANS_DISTANCE::ED_HSV_SVD) {
    auto hsva_img = ip::rgba_to_hsva(img.rgba_img);
    auto out_img = std::make_shared<Matrix<uint32_t>>(hsva_img->get_rows(), hsva_img->get_cols());
    kmeans.process_kmeans(hsva_img, out_img);
    final_img = ip::hsva_to_rgba(out_img);
  } else {
    kmeans.process_kmeans(img.rgba_img, final_img);
  }
  context.add_image(output_img_name, Image(final_img));
  return true;
}
