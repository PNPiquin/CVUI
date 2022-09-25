#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/togglebutton.h>

#include "ui/property.h"

class PropertyManager
{
public:
  PropertyManager(std::string manager_name = "Property manager");
  ~PropertyManager();

  Gtk::Box& get_widget() { return expander_box; };
  Gtk::ToggleButton& get_toggle() { return expander_toggle; };

  std::string get_enum_value(std::string property_name);
  void add_enum_property(std::string property_name, std::vector<std::string> values);

  bool get_boolean_value(std::string property_name);
  void add_boolean_property(std::string property_name, bool default_value);

  int get_integer_value(std::string property_name);
  void add_integer_property(std::string property_name, int default_value);

  double get_double_value(std::string property_name);
  void add_double_property(std::string property_name, double default_value);

  std::string get_string_value(std::string property_name);
  void add_string_property(std::string property_name, std::string default_value);

  void toggle_visibility();

  void add_button(std::shared_ptr<Gtk::Button> button);

  void add_separator();

private:
  std::map<std::string, std::shared_ptr<EnumProperty>> enum_properties;
  std::map<std::string, std::shared_ptr<BooleanProperty>> boolean_properties;
  std::map<std::string, std::shared_ptr<IntegerProperty>> integer_properties;
  std::map<std::string, std::shared_ptr<DoubleProperty>> double_properties;
  std::map<std::string, std::shared_ptr<StringProperty>> string_properties;

  // A property manager is an expander with a BoxLayout inside
  Gtk::Box expander_box, properties_box;
  Gtk::ToggleButton expander_toggle;

  // Style our toggle button
  Gtk::Box toggle_box;
  Gtk::Image icon_image;
  Gtk::Label toggle_label;

  // Styling
  Gtk::Separator separator;

  // Whether to show or hide all properties
  bool visible;
};

#endif