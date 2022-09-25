#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>

#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>

enum PropertyType
{
  BOOLEAN,
  INT,
  DOUBLE,
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

class IntegerProperty
{
public:
  IntegerProperty(std::string propert_name = "Integer property", int default_value = 0);
  ~IntegerProperty();

  int get_value();
  void set_value(int value);

  Gtk::Widget& get_widget() { return box; };

private:
  Glib::RefPtr<Gtk::Adjustment> spin_button_adjustment;
  Gtk::SpinButton spin_button;
  Gtk::Label property_label;
  Gtk::Box box;
};

class DoubleProperty
{
public:
  DoubleProperty(std::string propert_name = "Double property", double default_value = 0.);
  ~DoubleProperty();

  double get_value();
  void set_value(double value);

  Gtk::Widget& get_widget() { return box; };

private:
  Glib::RefPtr<Gtk::Adjustment> spin_button_adjustment;
  Gtk::SpinButton spin_button;
  Gtk::Label property_label;
  Gtk::Box box;
};

class StringProperty
{
public:
  StringProperty(std::string property_name = "String property", std::string default_value = "");
  ~StringProperty();

  std::string get_value();
  void set_value(std::string value);

  Gtk::Widget& get_widget() { return box; };

private:
  Gtk::Entry entry;
  Gtk::Label property_label;
  Gtk::Box box;
};

class EnumProperty
{
public:
  EnumProperty(std::string property_name = "Enum property",
               std::vector<std::string> values = std::vector<std::string>());
  ~EnumProperty();

  std::string get_value();
  void set_value(std::string value);

  Gtk::Widget& get_widget() { return box; };

private:
  Gtk::ComboBoxText combobox;
  Gtk::Label property_label;
  Gtk::Box box;
};

#endif