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
