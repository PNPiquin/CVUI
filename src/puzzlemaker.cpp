#include "puzzlemaker.h"
#include "utils/pixbuf.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

#include "image_processing/zone_utils.h"

static const int MARGIN = 10;

PuzzleMaker::PuzzleMaker()
  : properties_box(Gtk::Orientation::VERTICAL)
  , main_box(Gtk::Orientation::VERTICAL)
  , entry_box(Gtk::Orientation::HORIZONTAL)
  , m_paned(Gtk::Orientation::HORIZONTAL)
  , m_button("Load")
  , m_kmeans_button("Process Framing")
  , m_input_path_entry()
  , raw_pixbuf()
  , m_image()
  , is_processing(false)
{
  // Init private attributes
  input_path = "";

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  m_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_button_clicked));
  m_kmeans_button.signal_clicked().connect(sigc::mem_fun(*this, &PuzzleMaker::on_kmeans_button_clicked));

  // Build property tree
  build_property_tree();

  // Layout V2: a main vertical box with 2 children:
  //     - an horizontal box with all image entry/selection
  //     - a Gtk::Paned to display both the image and the properties
  // Build the entry box
  entry_box.append(m_input_path_entry);
  entry_box.append(m_button);
  entry_box.append(m_kmeans_button);

  // Build the Gtk::Paned
  m_paned.set_start_child(properties_box);
  m_image.set_size_request(800, -1);
  m_paned.set_end_child(m_image);

  // Append to main layout
  main_box.append(entry_box);
  main_box.append(m_paned);
  set_child(main_box);

  // Set fullscreen
  maximize();
  width = max_width;
  height = max_height;
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

std::string PuzzleMaker::get_current_filename()
{
  // Extract image path
  input_path = m_input_path_entry.get_buffer()->get_text();
  return std::filesystem::path(input_path).stem().string();
}