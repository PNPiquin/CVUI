#ifndef UTILS_PIXBUF_H
#define UTILS_PIXBUF_H

#include <cstdlib>
#include <memory>

#include <gdkmm/pixbuf.h>
#include <gtkmm.h>

#include "utils/matrix.h"

namespace utils {
Glib::RefPtr<Gdk::Pixbuf> resize_pixbuf(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf,
                                        int target_width,
                                        int max_width,
                                        int max_height);

Matrix<uint8_t> pixbuf_to_gray_mat(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf);
Matrix<uint32_t> pixbuf_to_rgba_mat(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf);

Glib::RefPtr<Gdk::Pixbuf> gray_mat_to_pixbuf(std::shared_ptr<Matrix<uint8_t>> gray_mat);
Glib::RefPtr<Gdk::Pixbuf> rgba_mat_to_pixbuf(std::shared_ptr<Matrix<uint32_t>> rgba_mat);
}

#endif