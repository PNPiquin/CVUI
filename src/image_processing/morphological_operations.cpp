#include "image_processing/morphological_operations.h"

#include <algorithm>
#include <vector>

#include "image_processing/utils.h"

namespace ip {
std::shared_ptr<Matrix<uint8_t>> apply_h_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size)
{
  auto output_img = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());
  for (size_t row = 0; row < input_img->get_rows() - gradient_size + 1; ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      std::vector<size_t> buffer(gradient_size);
      for (size_t delta = 0; delta < gradient_size; ++delta) {
        buffer.push_back(input_img->operator()(row + delta, col));
      }
      auto min_max = std::minmax_element(buffer.begin(), buffer.end());
      output_img->operator()(row, col) = *min_max.second - *min_max.first;
    }
  }
  return output_img;
}

std::shared_ptr<Matrix<uint8_t>> apply_v_gradient(std::shared_ptr<Matrix<uint8_t>> input_img, size_t gradient_size)
{
  auto output_img = std::make_shared<Matrix<uint8_t>>(input_img->get_rows(), input_img->get_cols());
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols() - gradient_size + 1; ++col) {
      std::vector<size_t> buffer(gradient_size);
      for (size_t delta = 0; delta < gradient_size; ++delta) {
        buffer.push_back(input_img->operator()(row, col + delta));
      }
      auto min_max = std::minmax_element(buffer.begin(), buffer.end());
      output_img->operator()(row, col) = *min_max.second - *min_max.first;
    }
  }
  return output_img;
}

std::shared_ptr<Matrix<uint32_t>> apply_h_gradient(std::shared_ptr<Matrix<uint32_t>> input_img, size_t gradient_size)
{
  auto output_img = std::make_shared<Matrix<uint32_t>>(input_img->get_rows(), input_img->get_cols());
  std::vector<size_t> buffer_r(gradient_size), buffer_b(gradient_size), buffer_g(gradient_size);
  for (size_t row = 0; row < input_img->get_rows() - gradient_size + 1; ++row) {
    for (size_t col = 0; col < input_img->get_cols(); ++col) {
      for (size_t delta = 0; delta < gradient_size; ++delta) {
        RGBAPixel pix(input_img->operator()(row + delta, col));
        buffer_r[delta] = pix.r;
        buffer_g[delta] = pix.g;
        buffer_b[delta] = pix.b;
      }
      auto min_max_r = std::minmax_element(buffer_r.begin(), buffer_r.end());
      auto min_max_g = std::minmax_element(buffer_g.begin(), buffer_g.end());
      auto min_max_b = std::minmax_element(buffer_b.begin(), buffer_b.end());
      output_img->operator()(row, col) = RGBAPixel(*min_max_r.second - *min_max_r.first,
                                                   *min_max_g.second - *min_max_g.first,
                                                   *min_max_b.second - *min_max_b.first)
                                           .to_uint32_t();
    }
  }
  return output_img;
}

std::shared_ptr<Matrix<uint32_t>> apply_v_gradient(std::shared_ptr<Matrix<uint32_t>> input_img, size_t gradient_size)
{
  auto output_img = std::make_shared<Matrix<uint32_t>>(input_img->get_rows(), input_img->get_cols());
  std::vector<size_t> buffer_r(gradient_size), buffer_b(gradient_size), buffer_g(gradient_size);
  for (size_t row = 0; row < input_img->get_rows(); ++row) {
    for (size_t col = 0; col < input_img->get_cols() - gradient_size + 1; ++col) {
      for (size_t delta = 0; delta < gradient_size; ++delta) {
        RGBAPixel pix(input_img->operator()(row, col + delta));
        buffer_r[delta] = pix.r;
        buffer_g[delta] = pix.g;
        buffer_b[delta] = pix.b;
      }
      auto min_max_r = std::minmax_element(buffer_r.begin(), buffer_r.end());
      auto min_max_g = std::minmax_element(buffer_g.begin(), buffer_g.end());
      auto min_max_b = std::minmax_element(buffer_b.begin(), buffer_b.end());
      output_img->operator()(row, col) = RGBAPixel(*min_max_r.second - *min_max_r.first,
                                                   *min_max_g.second - *min_max_g.first,
                                                   *min_max_b.second - *min_max_b.first)
                                           .to_uint32_t();
    }
  }
  return output_img;
}
}
