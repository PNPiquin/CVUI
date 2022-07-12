#include "ui/property_manager.h"

PropertyManager::PropertyManager(std::string manager_name)
  : expander_box(Gtk::Orientation::VERTICAL)
  , properties_box(Gtk::Orientation::VERTICAL)
  , expander_toggle(manager_name)
  , visible(false)
{
  // Plug toggle signal
  expander_toggle.signal_clicked().connect(sigc::mem_fun(*this, &PropertyManager::toggle_visibility));

  // Add everything to main box
  expander_box.append(expander_toggle);
  expander_box.append(properties_box);

  // By default, do not show properties
  properties_box.hide();
}

PropertyManager::~PropertyManager() {}

void PropertyManager::toggle_visibility()
{
  if (expander_toggle.get_active()) {
    properties_box.show();
  } else {
    properties_box.hide();
  }
}

bool PropertyManager::get_boolean_value(std::string property_name)
{
  auto b_prop = boolean_properties.find(property_name);
  if (b_prop == boolean_properties.end()) {
    return false;
  }
  return b_prop->second->get_value();
}

void PropertyManager::add_boolean_property(std::string property_name, bool default_value)
{
  std::shared_ptr<BooleanProperty> b_prop = std::make_shared<BooleanProperty>(property_name, default_value);
  boolean_properties.insert({ property_name, b_prop });
  properties_box.append(b_prop->get_widget());
}

int PropertyManager::get_integer_value(std::string property_name)
{
  auto int_prop = integer_properties.find(property_name);
  if (int_prop == integer_properties.end()) {
    return 0;
  }
  return int_prop->second->get_value();
}

void PropertyManager::add_integer_property(std::string property_name, int default_value)
{
  std::shared_ptr<IntegerProperty> int_prop = std::make_shared<IntegerProperty>(property_name, default_value);
  integer_properties.insert({ property_name, int_prop });
  properties_box.append(int_prop->get_widget());
}

double PropertyManager::get_double_value(std::string property_name)
{
  auto double_prop = double_properties.find(property_name);
  if (double_prop == double_properties.end()) {
    return 0.;
  }
  return double_prop->second->get_value();
}

void PropertyManager::add_double_property(std::string property_name, double default_value)
{
  std::shared_ptr<DoubleProperty> double_prop = std::make_shared<DoubleProperty>(property_name, default_value);
  double_properties.insert({ property_name, double_prop });
  properties_box.append(double_prop->get_widget());
}

std::string PropertyManager::get_string_value(std::string property_name)
{
  auto string_prop = string_properties.find(property_name);
  if (string_prop == string_properties.end()) {
    return "";
  }
  return string_prop->second->get_value();
}

void PropertyManager::add_string_property(std::string property_name, std::string default_value)
{
  std::shared_ptr<StringProperty> string_prop = std::make_shared<StringProperty>(property_name, default_value);
  string_properties.insert({ property_name, string_prop });
  properties_box.append(string_prop->get_widget());
}
