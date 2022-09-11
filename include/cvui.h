#ifndef CVUI_H
#define CVUI_H

#include <atomic>
#include <cstdlib>
#include <gdkmm/pixbuf.h>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/picture.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/window.h>
#include <map>
#include <memory>
#include <string>

#include "pipeline/configuration.h"
#include "pipeline/context.h"
#include "pipeline/image_processing/border_processor.h"
#include "pipeline/image_processing/framing_processor.h"
#include "pipeline/processor.h"

#include "ui/constants.h"
#include "ui/property_manager.h"

#include "utils/matrix.h"

class CVUI : public Gtk::Window
{

public:
  CVUI();
  virtual ~CVUI();

protected:
  // Signal handlers:
  void on_button_clicked();
  void on_kmeans_button_clicked();

  // Layout
  Gtk::ScrolledWindow properties_scroll;
  Gtk::Box properties_box;

  // Layout V2
  Gtk::Box main_box, entry_box;
  Gtk::Paned m_paned;

  // Member widgets:
  Gtk::Button m_button;
  Gtk::Button m_kmeans_button;
  Gtk::Entry m_input_path_entry;

  // Properties
  std::map<std::string, std::shared_ptr<PropertyManager>> property_managers;

  // Image management
  Glib::RefPtr<Gdk::Pixbuf> raw_pixbuf;
  Gtk::Picture m_image;

  std::vector<std::shared_ptr<Gtk::Button>> buttons;

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
  Context context;

  void init_monitor_size();
  void build_property_tree();

  std::string get_current_filename();

  // Image processing threads
  void process_kmeans();

  // Processors
  FramingProcessor framing_processor;
  BorderProcessor border_processor;

  // Processor registration
  std::shared_ptr<PropertyManager> register_processor(std::string processor_display_name, BaseProcessor& processor);
};

#endif