#include "pipeline/configuration.h"

Configuration::Configuration() {}

void Configuration::set_boolean_property(std::string property_name, bool value)
{
  // Check if property name is already used. If yes do replace value
  auto it = boolean_properties.find(property_name);
  if (it == boolean_properties.end()) {
    boolean_properties.insert({ property_name, value });
  } else {
    it->second = value;
  }
}

void Configuration::set_integer_property(std::string property_name, int value)
{
  // Check if property name is already used. If yes do nothing
  auto it = integer_properties.find(property_name);
  if (it == integer_properties.end()) {
    integer_properties.insert({ property_name, value });
  } else {
    it->second = value;
  }
}

void Configuration::set_double_property(std::string property_name, double value)
{
  // Check if property name is already used. If yes do nothing
  auto it = double_properties.find(property_name);
  if (it == double_properties.end()) {
    double_properties.insert({ property_name, value });
  } else {
    it->second = value;
  }
}

void Configuration::set_string_property(std::string property_name, std::string value)
{
  // Check if property name is already used. If yes do nothing
  auto it = string_properties.find(property_name);
  if (it == string_properties.end()) {
    string_properties.insert({ property_name, value });
  } else {
    it->second = value;
  }
}

bool Configuration::get_bool(std::string property_name)
{
  auto it = boolean_properties.find(property_name);
  if (it == boolean_properties.end()) {
    return false;
  }

  return it->second;
}

int Configuration::get_int(std::string property_name)
{
  auto it = integer_properties.find(property_name);
  if (it == integer_properties.end()) {
    return 0;
  }

  return it->second;
}

double Configuration::get_double(std::string property_name)
{
  auto it = double_properties.find(property_name);
  if (it == double_properties.end()) {
    return 0.;
  }

  return it->second;
}

std::string Configuration::get_string(std::string property_name)
{
  auto it = string_properties.find(property_name);
  if (it == string_properties.end()) {
    return "";
  }

  return it->second;
}

std::vector<std::string> Configuration::get_boolean_properties_names(){
  std::vector<std::string> boolean_properties_names;
  for (auto it = boolean_properties.begin(); it != boolean_properties.end(); ++it){
    boolean_properties_names.push_back(it->first);
  }
  return boolean_properties_names;
}

std::vector<std::string> Configuration::get_integer_properties_names(){
  std::vector<std::string> integer_properties_names;
  for (auto it = integer_properties.begin(); it != integer_properties.end(); ++it){
    integer_properties_names.push_back(it->first);
  }
  return integer_properties_names;
}

std::vector<std::string> Configuration::get_double_properties_names(){
  std::vector<std::string> double_properties_names;
  for (auto it = double_properties.begin(); it != double_properties.end(); ++it){
    double_properties_names.push_back(it->first);
  }
  return double_properties_names;
}

std::vector<std::string> Configuration::get_string_properties_names(){
  std::vector<std::string> string_properties_names;
  for (auto it = string_properties.begin(); it != string_properties.end(); ++it){
    string_properties_names.push_back(it->first);
  }
  return string_properties_names;
}