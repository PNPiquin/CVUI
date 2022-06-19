#ifndef PUZZLEMAKER_H
#define PUZZLEMAKER_H

#include <cstdlib>
#include <gdkmm/pixbuf.h>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/window.h>
#include <string>

#include "utils/matrix.h"

class PuzzleMaker : public Gtk::Window
{

public:
  PuzzleMaker();
  virtual ~PuzzleMaker();

protected:
  // Signal handlers:
  void on_button_clicked();
  bool on_configure_changed(GdkEventConfigure* configure_event);

  // Layout
  Gtk::Grid m_grid;

  // Member widgets:
  Gtk::Button m_button;
  Gtk::Entry m_input_path_entry;

  // Image management
  Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf;
  Gtk::Image m_image;

private:
  // Window size
  int width;
  int height;
  int max_width;
  int max_height;

  // entry text
  std::string input_path;

  void on_resize(int new_width, int new_height);

  // Image
  Matrix<uint8_t> gray_img;
  Matrix<uint32_t> rgba_img;
};

#endif