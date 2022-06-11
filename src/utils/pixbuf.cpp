#include "utils/pixbuf.h"

namespace utils {
Glib::RefPtr<Gdk::Pixbuf> resize_pixbuf(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf, int window_width, int target_width)
{
  Glib::RefPtr<Gdk::Pixbuf> resized_pixbuf = raw_pixbuf;
  if (raw_pixbuf->get_width() > window_width) {
    float ratio = window_width / float(raw_pixbuf->get_width());
    resized_pixbuf =
      raw_pixbuf->scale_simple(window_width, int(raw_pixbuf->get_height() * ratio), Gdk::InterpType::INTERP_NEAREST);
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
      mat(row, col) = pixel[0] >> 24 || pixel[1] >> 16 || pixel[2] >> 8 || pixel[3];
    }
  }
  return mat;
}

}
