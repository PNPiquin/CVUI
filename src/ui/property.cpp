#include "ui/property.h"

// ------------------------------------------------------------------------------------------------
//                                     BOOLEAN PROPERTY
// ------------------------------------------------------------------------------------------------
BooleanProperty::BooleanProperty(std::string property_name, bool default_value)
  : check_button()
{
  check_button.set_label(property_name);
  check_button.set_active(default_value);
}

BooleanProperty::~BooleanProperty() {}

bool BooleanProperty::get_value()
{
  return check_button.get_active();
}

void BooleanProperty::set_value(bool value)
{
  check_button.set_active(value);
}
