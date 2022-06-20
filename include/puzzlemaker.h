#ifndef PUZZLEMAKER_H
#define PUZZLEMAKER_H

#include <atomic>
#include <cstdlib>
#include <gdkmm/pixbuf.h>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/window.h>
#include <string>


#include "utils/context.h"
#include "utils/matrix.h"

class PuzzleMaker : public Gtk::Window
{

public:
  PuzzleMaker();
  virtual ~PuzzleMaker();

protected:
  // Signal handlers:
  void on_button_clicked();
  void on_kmeans_button_clicked();
  bool on_configure_changed(GdkEventConfigure* configure_event);

  // Layout
  Gtk::Grid m_grid;

  // Member widgets:
  Gtk::Button m_button;
  Gtk::Button m_kmeans_button;
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

  // thread management
  std::atomic<bool> is_processing;

  // entry text
  std::string input_path;

  // Image
  GrayContext gray_context;
  RGBAContext rgba_context;

  void on_resize(int new_width, int new_height);
  std::string get_current_filename();

  // Image processing threads
  void process_kmeans();
};

#endif