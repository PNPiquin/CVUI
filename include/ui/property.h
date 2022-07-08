#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>

#include <gtkmm/checkbutton.h>

enum PropertyType
{
  BOOLEAN,
  INT,
  FLOAT,
  STRING,
};

class BooleanProperty
{
public:
  BooleanProperty(std::string propert_name = "Boolean property", bool default_value = false);
  ~BooleanProperty();

  bool get_value();
  void set_value(bool value);

  Gtk::CheckButton& get_widget() { return check_button; };

private:
  Gtk::CheckButton check_button;
};

#endif