#ifndef PIPELINE_CONFIGURATION_H
#define PIPELINE_CONFIGURATION_H

#include <map>
#include <string>
#include <vector>

class EnumType
{
public:
  EnumType();
  ~EnumType() = default;

  void add_value(std::string new_value);
  void remove_value(std::string value_to_remove);

  std::string get_value() { return current_value; };
  void set_value(std::string value) { current_value = value; };

  std::vector<std::string> get_values() { return values; };

private:
  std::string current_value;
  std::vector<std::string> values;
};

class Configuration
{
public:
  Configuration();
  ~Configuration() = default;

  void set_enum_property(std::string property_name, EnumType _enum = EnumType());
  void set_boolean_property(std::string property_name, bool value = false);
  void set_integer_property(std::string property_name, int value = 0);
  void set_double_property(std::string property_name, double value = 0.);
  void set_string_property(std::string property_name, std::string value = "");

  void set_enum_value(std::string property_name, std::string enum_value);
  void set_bool(std::string property_name, bool value);
  void set_int(std::string property_name, int value);
  void set_double(std::string property_name, double value);
  void set_string(std::string property_name, std::string value);

  std::string get_enum_value(std::string property_name);
  bool get_bool(std::string property_name);
  int get_int(std::string property_name);
  double get_double(std::string property_name);
  std::string get_string(std::string property_name);

  // In enum case, we also want to access all enum possible values
  std::vector<std::string> get_enum_values(std::string property_name);

  std::vector<std::string> get_enum_properties_names();
  std::vector<std::string> get_boolean_properties_names();
  std::vector<std::string> get_integer_properties_names();
  std::vector<std::string> get_double_properties_names();
  std::vector<std::string> get_string_properties_names();

private:
  std::map<std::string, EnumType> enum_properties;
  std::map<std::string, bool> boolean_properties;
  std::map<std::string, int> integer_properties;
  std::map<std::string, double> double_properties;
  std::map<std::string, std::string> string_properties;
};

#endif