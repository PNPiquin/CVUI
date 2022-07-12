#include "puzzlemaker.h"
#include "utils/pixbuf.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

#include "image_processing/zone_utils.h"

static const int MARGIN = 10;

PuzzleMaker::PuzzleMaker()
  : m_grid()
  , properties_box(Gtk::Orientation::VERTICAL)
  , m_button("Load")
  , m_kmeans_button("Process Framing")
  , m_input_path_entry()
  , raw_pixbuf()
  , m_image()
  , is_processing(false)
{
  // Init private attributes
  width = 600;
  height = 200;
  input_path = "";

  // Sets the border width of the window.
  set_default_size(width, height);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_button_clicked));
  m_kmeans_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_kmeans_button_clicked));

  // Build property tree
  build_property_tree();

  // This packs the button into the Window (a container).
  m_grid.attach(m_input_path_entry, 1, 0, 3, 1);
  m_grid.attach(m_button, 4, 0, 1, 1);
  m_grid.attach(m_kmeans_button, 5, 0, 1, 1);
  m_grid.attach(m_image, 1, 1, 5, 5);
  m_grid.attach(properties_box, 0, 0, 1, 6);
  set_child(m_grid);

  // The final step is to display this newly created widget...
  m_button.show();
}

PuzzleMaker::~PuzzleMaker() {}

void PuzzleMaker::init_monitor_size()
{
  // Get display size
  auto display = this->get_display();
  auto monitor = display->get_monitor_at_surface(this->get_surface());
  Gdk::Rectangle monitor_rect;
  monitor->get_geometry(monitor_rect);
  max_width = monitor_rect.get_width();
  max_height = monitor_rect.get_height();
}

void PuzzleMaker::build_property_tree()
{
  // PuzzleMaker properties
  std::shared_ptr<PropertyManager> puzzle_maker_properties = std::make_shared<PropertyManager>(PUZZLEMAKER);
  property_managers.insert({ PUZZLEMAKER, puzzle_maker_properties });
  puzzle_maker_properties->add_boolean_property("Test toggle", true);
  puzzle_maker_properties->add_integer_property("Test spin button", 2);
  puzzle_maker_properties->add_double_property("Test double spin button", 42.3);
  puzzle_maker_properties->add_string_property("Test entry", "Entry test");
  properties_box.append(puzzle_maker_properties->get_widget());
}

void PuzzleMaker::on_button_clicked()
{
  init_monitor_size();

  std::string filename = get_current_filename();
  // Load and resize raw image
  raw_pixbuf = Gdk::Pixbuf::create_from_file(input_path);

  // Store raw image for further processing
  gray_context.add_image(filename + "_gray", std::make_shared<Matrix<uint8_t>>(utils::pixbuf_to_gray_mat(raw_pixbuf)));
  rgba_context.add_image(filename, std::make_shared<Matrix<uint32_t>>(utils::pixbuf_to_rgba_mat(raw_pixbuf)));

  // Set image
  m_image.set_pixbuf(raw_pixbuf);
  m_image.set_expand(true);
  m_image.set_can_shrink(true);
  m_image.set_keep_aspect_ratio(true);

  // Get size request
  int target_width = std::min((max_width - 2 * MARGIN) / 2, raw_pixbuf->get_width());
  int target_height = std::min((max_height - 2 * MARGIN - 100) / 2, raw_pixbuf->get_height());
  m_image.set_size_request(target_width, target_height);
}

void PuzzleMaker::on_kmeans_button_clicked()
{
  if (!is_processing) {
    std::thread kmeans_thread(&PuzzleMaker::process_kmeans, this);
    kmeans_thread.detach();
  }
}

void PuzzleMaker::process_kmeans()
{
  is_processing = true;

  std::string filename = get_current_filename();
  std::string gray_filename = filename + "_gray";

  // generate kmeans image
  std::string framing_img_name = gray_filename + "_framing";
  // gray_context.generate_random_framing(gray_filename, framing_img_name);
  gray_context.generate_grid_framing(gray_filename, framing_img_name);
  gray_context.save_image(framing_img_name);

  // generate border image
  std::string border_img_name = framing_img_name + "_borders";
  gray_context.generate_border_image(framing_img_name, border_img_name);
  gray_context.save_image(border_img_name);

  // apply on RGBA picture
  std::string rgba_with_framing_img_name = filename + "_framing_overlay";
  rgba_context.apply_framing(filename, gray_context.get_image(border_img_name), rgba_with_framing_img_name);
  rgba_context.save_image(rgba_with_framing_img_name);

  // extract images
  auto sub_images =
    ip::extract_zone_images(rgba_context.get_image(filename), gray_context.get_image(framing_img_name), 120);
  int i = 0;
  for (const auto& sub_img : sub_images) {
    RGBAContext::save_image(sub_img, "fragments/" + filename + std::to_string(i) + ".png");
    ++i;
  }

  printf("DONE\n");

  is_processing = false;
}

// bool PuzzleMaker::on_configure_changed(GdkEventConfigure* configure_event)
// {
//   if (configure_event->width != width || configure_event->height != height) {
//     on_resize(configure_event->width, configure_event->height);
//   }
//   return false;
// }

void PuzzleMaker::on_resize(int new_width, int new_height)
{
  width = new_width;
  height = new_height;
  int image_target_width = width - 2 * MARGIN;

  if (raw_pixbuf) {
    m_image.set_pixbuf(
      utils::resize_pixbuf(raw_pixbuf, image_target_width, max_width - 2 * MARGIN, max_height - 2 * MARGIN - 100));
    m_image.set_size_request(50, 50);
  }
}

std::string PuzzleMaker::get_current_filename()
{
  // Extract image path
  input_path = m_input_path_entry.get_buffer()->get_text();
  return std::filesystem::path(input_path).stem().string();
}