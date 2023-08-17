#include "pipeline/image_processing/complex/crestline_detector.h"

#include "image_processing/canny.h"
#include "image_processing/morphological_operations.h"
#include "image_processing/normalization.h"
#include "image_processing/object_detection/blob_detection.h"
#include "image_processing/spatial_filtering.h"
#include "image_processing/utils.h"

CrestlineDetector::CrestlineDetector()
{
  processor_name = "Crestline detector";
  processor_suffix = "_crest";

  // Blob extraction
  config.set_double_property(CD_X_MIN_SPAN, 0.);
  config.set_double_property(CD_Y_MIN_SPAN, 200.);
  config.set_double_property(CD_XY_MIN_RATIO, 0.);
  config.set_double_property(CD_XY_MAX_RATIO, 100.);

  // Dilatation
  config.set_integer_property(CD_DILATATION_SIZE, 2);
  config.set_integer_property(CD_DILATATION_REPETITTION, 6);

  // Canny
  config.set_double_property(CD_CANNY_LOW_THRESHOLD, 23.);
  config.set_double_property(CD_CANNY_HIGH_THRESHOLD, 100.);

  // Gaussian kernel
  config.set_integer_property(CD_KERNEL_SIZE, 5);
  config.set_double_property(CD_KERNEL_STD, 1.0);

  // Post-treatment
  config.set_integer_property(CD_KEEP_TOP_PIXELS, 10);
}

bool CrestlineDetector::process(Context& context, std::string img_name, std::string output_img_name)
{
  auto img = context.get_image(img_name);
  if (img.type == ImageType::UNKNOWN || img.type == ImageType::GRAY) {
    return false;
  }

  // Gaussian blur
  auto kernel = create_kernel(config);
  auto blur1_img = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
  auto blur2_img = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
  ip::apply_kernel(img.rgba_img, kernel, blur1_img);
  ip::apply_kernel(blur1_img, kernel, blur2_img);

  // Canny edge detection
  double low_threshold = config.get_double(CD_CANNY_LOW_THRESHOLD);
  double high_threshold = config.get_double(CD_CANNY_HIGH_THRESHOLD);
  CannyEdgeDetector canny_edge_detector(low_threshold, high_threshold);
  canny_edge_detector.process_rgba_img(blur2_img);
  auto edge_img = canny_edge_detector.get_canny_edges();

  // Morphological dilatation
  int dilatation_size = config.get_int(CD_KERNEL_SIZE);
  int number_of_dilatations = config.get_int(CD_DILATATION_REPETITTION);
  auto dilated_img = edge_img;
  for (int i = 0; i < number_of_dilatations; i++) {
    dilated_img = ip::apply_dilatation(dilated_img, dilatation_size);
  }

  // Blob extraction configuration
  float x_min_span = config.get_double(CD_X_MIN_SPAN);
  float y_min_span = config.get_double(CD_Y_MIN_SPAN);
  float xy_min_ratio = config.get_double(CD_XY_MIN_RATIO);
  float xy_max_ratio = config.get_double(CD_XY_MAX_RATIO);

  BlobDetector blob_detector(x_min_span, y_min_span, xy_min_ratio, xy_max_ratio);
  auto res_img = blob_detector.process(dilated_img);

  // Post-treatment
  int n_pixels_to_keep = config.get_int(CD_KEEP_TOP_PIXELS);
  if (n_pixels_to_keep > 0) {
    res_img = select_top_pixels(res_img, n_pixels_to_keep);
  }

  context.add_image(output_img_name, Image(res_img));

  // To keep texture below the crestline:
  // - clasic edge kernel on input image -> keep only what is below the crestline
  // - normalize result to 70% of crestline intensity
  auto laplacian_kernel = ip::create_laplacian_kernel();
  auto laplacian_img_raw = std::make_shared<Matrix<uint32_t>>(img.rgba_img->get_rows(), img.rgba_img->get_cols());
  auto laplacian_img = ip::min_max_normalization(laplacian_img_raw);
  ip::apply_kernel(img.rgba_img, laplacian_kernel, laplacian_img);
  auto texture_img = create_texture_img(res_img, laplacian_img);
  context.add_image(output_img_name + "_texture", Image(texture_img));

  return true;
}

// Private utils
std::shared_ptr<Matrix<float>> CrestlineDetector::create_kernel(Configuration& config)
{
  int kernel_size = config.get_int(CD_KERNEL_SIZE);
  double sigma = config.get_double(CD_KERNEL_STD);
  return ip::create_gaussian_kernel(kernel_size, sigma);
}

std::shared_ptr<Matrix<uint8_t>> CrestlineDetector::select_top_pixels(std::shared_ptr<Matrix<uint8_t>> input_img,
                                                                      int n_pixels)
{
  auto output_img = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());

  for (size_t col = 0; col < input_img->get_cols(); col++) {
    int current_number_of_pixel = 0;
    for (size_t row = 0; row < input_img->get_rows(); row++) {
      uint8_t pix = input_img->operator()(row, col);
      if (pix > 0) {
        output_img->operator()(row, col) = pix;
        current_number_of_pixel++;
        if (current_number_of_pixel >= n_pixels) {
          break;
        }
      }
    }
  }

  return output_img;
}

std::shared_ptr<Matrix<uint32_t>> CrestlineDetector::create_texture_img(std::shared_ptr<Matrix<uint8_t>> crestine_img,
                                                                        std::shared_ptr<Matrix<uint32_t>> laplacian_img)
{
  auto output_img = std::make_shared<Matrix<uint32_t>>(crestine_img->get_rows(), crestine_img->get_cols());

  for (size_t col = 0; col < output_img->get_cols(); col++) {
    bool reached_crestline = false;
    size_t from_crestline = 0;
    for (size_t row = 0; row < output_img->get_rows(); row++) {
      uint8_t pix = crestine_img->operator()(row, col);
      if (pix > 0) {
        reached_crestline = true;
        output_img->operator()(row, col) = RGBAPixel(pix, pix, pix).to_uint32_t();
      }

      if (reached_crestline) {
        from_crestline++;
      }

      if (from_crestline > 10) {
        output_img->operator()(row, col) = laplacian_img->operator()(row, col);
      }
    }
  }

  for (size_t col = 0; col < output_img->get_cols(); col++) {
    for (size_t row = 0; row < output_img->get_rows(); row++) {
      RGBAPixel pix = output_img->operator()(row, col);
      output_img->operator()(row, col) = RGBAPixel(255 - pix.r, 255 - pix.g, 255 - pix.b).to_uint32_t();
    }
  }
  return output_img;
}
