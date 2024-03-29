#include "utils/pixbuf.h"

namespace utils {
Glib::RefPtr<Gdk::Pixbuf> resize_pixbuf(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf,
                                        int target_width,
                                        int max_width,
                                        int max_height)
{
  Glib::RefPtr<Gdk::Pixbuf> resized_pixbuf = raw_pixbuf;
  if (raw_pixbuf->get_width() != target_width) {
    float ratio = target_width / float(raw_pixbuf->get_width());
    int final_target_width = std::min(target_width, max_width);
    int final_target_height = std::min(int(raw_pixbuf->get_height() * ratio), max_height);
    resized_pixbuf = raw_pixbuf->scale_simple(final_target_width, final_target_height, Gdk::InterpType::NEAREST);
  }
  return resized_pixbuf;
}

Matrix<uint8_t> pixbuf_to_gray_mat(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf)
{
  Matrix<uint8_t> mat(raw_pixbuf->get_height(), raw_pixbuf->get_width());
  auto pixels = raw_pixbuf->get_pixels();
  int rowstride = raw_pixbuf->get_rowstride();
  int n_channels = raw_pixbuf->get_n_channels();
  for (size_t row = 0; row < mat.get_rows(); ++row) {
    for (size_t col = 0; col < mat.get_cols(); ++col) {
      auto* pixel = pixels + row * rowstride + col * n_channels;
      mat(row, col) = uint8_t(0.2989 * pixel[0] + 0.5870 * pixel[1] + 0.1140 * pixel[2]);
    }
  }
  return mat;
}

Matrix<uint32_t> pixbuf_to_rgba_mat(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf)
{
  Matrix<uint32_t> mat(raw_pixbuf->get_height(), raw_pixbuf->get_width());
  auto pixels = raw_pixbuf->get_pixels();
  int rowstride = raw_pixbuf->get_rowstride();
  int n_channels = raw_pixbuf->get_n_channels();
  for (size_t row = 0; row < mat.get_rows(); ++row) {
    for (size_t col = 0; col < mat.get_cols(); ++col) {
      auto* pixel = pixels + row * rowstride + col * n_channels;
      mat(row, col) = pixel[0] << 24 | pixel[1] << 16 | pixel[2] << 8 | pixel[3];
    }
  }
  return mat;
}

Glib::RefPtr<Gdk::Pixbuf> gray_mat_to_pixbuf(std::shared_ptr<Matrix<uint8_t>> gray_mat)
{
  unsigned int cols = gray_mat->get_cols();
  unsigned int rows = gray_mat->get_rows();
  auto data = new uint8_t[cols * rows * 3];
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      data[(row * cols + col) * 3] = gray_mat->operator()(row, col);
      data[(row * cols + col) * 3 + 1] = gray_mat->operator()(row, col);
      data[(row * cols + col) * 3 + 2] = gray_mat->operator()(row, col);
    }
  }
  return Gdk::Pixbuf::create_from_data(
    data, Gdk::Colorspace::RGB, false, 8, cols, rows, cols * 3, [](const unsigned char* data) { delete[] data; });
}
Glib::RefPtr<Gdk::Pixbuf> rgba_mat_to_pixbuf(std::shared_ptr<Matrix<uint32_t>> rgba_mat)
{
  unsigned int cols = rgba_mat->get_cols();
  unsigned int rows = rgba_mat->get_rows();
  auto data = new uint8_t[cols * rows * 3];
  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < cols; ++col) {
      uint32_t rgba_pixel = rgba_mat->operator()(row, col);
      data[(row * cols + col) * 3] = uint8_t((rgba_pixel & 0xff000000) >> 24);
      data[(row * cols + col) * 3 + 1] = uint8_t((rgba_pixel & 0x00ff0000) >> 16);
      data[(row * cols + col) * 3 + 2] = uint8_t((rgba_pixel & 0x0000ff00) >> 8);
      // data[(row * cols + col) * 4 + 3] = uint8_t((rgba_pixel & 0x000000ff));
    }
  }
  return Gdk::Pixbuf::create_from_data(
    data, Gdk::Colorspace::RGB, false, 8, cols, rows, cols * 3, [](const unsigned char* data) { delete[] data; });
}

}
