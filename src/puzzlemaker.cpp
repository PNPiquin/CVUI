#include "puzzlemaker.h"
#include "utils/pixbuf.h"
#include <filesystem>
#include <iostream>
#include <memory>

static const int MARGIN = 10;

PuzzleMaker::PuzzleMaker()
  : m_grid()
  , m_button("Load")
  , m_kmeans_button("Process Kmeans")
  , m_input_path_entry()
  , raw_pixbuf()
  , m_image()
{
  // Init private attributes
  width = 600;
  height = 200;
  max_width = gdk_screen_width();
  max_height = gdk_screen_height();
  input_path = "";

  // Sets the border width of the window.
  set_border_width(MARGIN);
  set_default_size(width, height);

  // Configure configuration callbacks
  signal_configure_event().connect(sigc::mem_fun(*this, &PuzzleMaker::on_configure_changed), false);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_button_clicked));
  m_kmeans_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_kmeans_button_clicked));

  // This packs the button into the Window (a container).
  m_grid.attach(m_input_path_entry, 0, 0, 3, 1);
  m_grid.attach(m_button, 3, 0, 1, 1);
  m_grid.attach(m_kmeans_button, 4, 0, 1, 1);
  m_grid.attach(m_image, 0, 1, 5, 5);
  m_grid.show_all();
  add(m_grid);

  // The final step is to display this newly created widget...
  m_button.show();
}

PuzzleMaker::~PuzzleMaker() {}

void PuzzleMaker::on_button_clicked()
{
  std::string filename = get_current_filename();
  // Load and resize raw image
  raw_pixbuf = Gdk::Pixbuf::create_from_file(input_path);

  // Store raw image for further processing
  gray_context.add_image(filename + "_gray", std::make_shared<Matrix<uint8_t>>(utils::pixbuf_to_gray_mat(raw_pixbuf)));
  rgba_context.add_image(filename, std::make_shared<Matrix<uint32_t>>(utils::pixbuf_to_rgba_mat(raw_pixbuf)));

  // Resize image
  int window_width = get_width() - 2 * MARGIN;
  auto pixbuf = utils::resize_pixbuf(raw_pixbuf, window_width, max_width - 2 * MARGIN, max_height - 2 * MARGIN - 100);
  // Set image
  m_image.set(pixbuf);
}

void PuzzleMaker::on_kmeans_button_clicked()
{
  printf("Process KMEANS");
  std::string filename = get_current_filename();
  filename = filename + "_gray";
  gray_context.process_kmeans(filename);
  gray_context.save_image(filename);
  gray_context.save_image(filename + "_kmeans");
}

bool PuzzleMaker::on_configure_changed(GdkEventConfigure* configure_event)
{
  if (configure_event->width != width || configure_event->height != height) {
    on_resize(configure_event->width, configure_event->height);
  }
  return false;
}

void PuzzleMaker::on_resize(int new_width, int new_height)
{
  width = new_width;
  height = new_height;
  int image_target_width = width - 2 * MARGIN;

  if (raw_pixbuf) {
    m_image.set(
      utils::resize_pixbuf(raw_pixbuf, image_target_width, max_width - 2 * MARGIN, max_height - 2 * MARGIN - 100));
  }
}

std::string PuzzleMaker::get_current_filename()
{
  // Extract image path
  input_path = m_input_path_entry.get_buffer()->get_text();
  std::cout << "Load file: " << input_path << std::endl;
  return std::filesystem::path(input_path).filename().string();
}