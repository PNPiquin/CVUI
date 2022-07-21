#ifndef PIPELINE_CONFIGURATION_H
#define PIPELINE_CONFIGURATION_H

#include <map>
#include <string>

class Configuration
{
public:
  Configuration();
  ~Configuration();

  void set_boolean_property(std::string property_name, bool value = false);
  void set_integer_property(std::string property_name, int value = 0);
  void set_double_property(std::string property_name, double value = 0.);
  void set_string_property(std::string property_name, std::string value = "");

  bool get_bool(std::string property_name);
  int get_int(std::string property_name);
  double get_double(std::string property_name);
  std::string get_string(std::string property_name);

private:
  std::map<std::string, bool> boolean_properties;
  std::map<std::string, int> integer_properties;
  std::map<std::string, double> double_properties;
  std::map<std::string, std::string> string_properties;
};

#endif