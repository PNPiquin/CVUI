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

// ------------------------------------------------------------------------------------------------
//                                     INTEGER PROPERTY
// ------------------------------------------------------------------------------------------------
IntegerProperty::IntegerProperty(std::string property_name, int default_value)
  : spin_button_adjustment(Gtk::Adjustment::create(default_value, 0, 1000))
  , spin_button(spin_button_adjustment)
  , property_label(property_name)
  , box(Gtk::Orientation::HORIZONTAL)
{
  box.append(property_label);
  box.append(spin_button);
}

IntegerProperty::~IntegerProperty() {}

int IntegerProperty::get_value()
{
  return spin_button.get_value_as_int();
}

void IntegerProperty::set_value(int value)
{
  spin_button.set_value(value);
}

// ------------------------------------------------------------------------------------------------
//                                     DOUBLE PROPERTY
// ------------------------------------------------------------------------------------------------
DoubleProperty::DoubleProperty(std::string property_name, double default_value)
  : spin_button_adjustment(Gtk::Adjustment::create(default_value, 0., 1000., 0.5))
  , spin_button(spin_button_adjustment, 0.5, 2)
  , property_label(property_name)
  , box(Gtk::Orientation::HORIZONTAL)
{
  box.append(property_label);
  box.append(spin_button);
}

DoubleProperty::~DoubleProperty() {}

double DoubleProperty::get_value()
{
  return spin_button.get_value();
}

void DoubleProperty::set_value(double value)
{
  spin_button.set_value(value);
}

// ------------------------------------------------------------------------------------------------
//                                     STRING PROPERTY
// ------------------------------------------------------------------------------------------------
StringProperty::StringProperty(std::string property_name, std::string default_value)
  : entry()
  , property_label(property_name)
  , box(Gtk::Orientation::HORIZONTAL)
{
  entry.set_text(default_value);

  box.append(property_label);
  box.append(entry);
}

StringProperty::~StringProperty() {}

std::string StringProperty::get_value()
{
  return entry.get_text();
}

void StringProperty::set_value(std::string value)
{
  entry.set_text(value);
}
