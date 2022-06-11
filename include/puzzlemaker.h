#ifndef PUZZLEMAKER_H
#define PUZZLEMAKER_H

#include <gdkmm/pixbuf.h>
#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/image.h>
#include <gtkmm/window.h>
#include <string>

class PuzzleMaker : public Gtk::Window
{

public:
  PuzzleMaker();
  virtual ~PuzzleMaker();

protected:
  // Signal handlers:
  void on_button_clicked();

  // Layout
  Gtk::Grid m_grid;

  // Member widgets:
  Gtk::Button m_button;
  Gtk::Entry m_input_path_entry;

  // Image management
  Gtk::Image m_image;

private:
  // entry text
  std::string input_path;
};

#endif