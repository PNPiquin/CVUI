#ifndef UTILS_PIXBUF_H
#define UTILS_PIXBUF_H

#include <gdkmm/pixbuf.h>
#include <gtkmm.h>

namespace utils {
Glib::RefPtr<Gdk::Pixbuf> resize_pixbuf(Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf, int window_width, int target_width);
}

#endif