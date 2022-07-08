#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include <gtkmm/box.h>
#include <gtkmm/togglebutton.h>

#include "ui/property.h"

class PropertyManager
{
public:
  PropertyManager(std::string manager_name = "Property manager");
  ~PropertyManager();

  Gtk::Widget& get_widget() { return expander_box; };
  Gtk::ToggleButton& get_toggle() { return expander_toggle; };

  bool get_boolean_value(std::string property_name);
  void add_boolean_property(std::string property_name, bool default_value);

  void toggle_visibility();

private:
  std::map<std::string, std::shared_ptr<BooleanProperty>> boolean_properties;

  // A property manager is an expander with a BoxLayout inside
  Gtk::Box expander_box, properties_box;
  Gtk::ToggleButton expander_toggle;

  // Whether to show or hide all properties
  bool visible;
};

#endif