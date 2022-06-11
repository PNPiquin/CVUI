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
}
