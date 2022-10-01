#include "pipeline/configuration.h"

EnumType::EnumType() {}

void EnumType::add_value(std::string new_value)
{
  // First check if value does not already exists. If it exists, no-op
  for (const auto& v : values) {
    if (v == new_value) {
      return;
    }
  }

  values.push_back(new_value);
}

void EnumType::remove_value(std::string value_to_remove)
{
  size_t idx_to_remove = 0;
  bool need_to_remove = false;
  for (size_t idx = 0; idx < values.size(); ++idx) {
    if (values[idx] == value_to_remove) {
      idx_to_remove = idx;
      need_to_remove = true;
      break;
    }
  }

  if (need_to_remove) {
    values.erase(values.begin() + idx_to_remove);
  }
}

Configuration::Configuration() {}

void Configuration::set_enum_property(std::string property_name, EnumType _enum)
{
  // Check if property name is already used. If yes do replace value
  auto it = enum_properties.find(property_name);
  if (it == enum_properties.end()) {
    enum_properties.insert({ property_name, _enum });
  } else {
    it->second = _enum;
  }
}

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

void Configuration::set_enum_value(std::string property_name, std::string enum_value)
{
  auto it = enum_properties.find(property_name);
  if (it == enum_properties.end()) {
    return;
  }

  it->second.set_value(enum_value);
}

void Configuration::set_bool(std::string property_name, bool value)
{
  auto it = boolean_properties.find(property_name);
  if (it == boolean_properties.end()) {
    return;
  }

  it->second = value;
}

void Configuration::set_int(std::string property_name, int value)
{
  auto it = integer_properties.find(property_name);
  if (it == integer_properties.end()) {
    return;
  }

  it->second = value;
}

void Configuration::set_double(std::string property_name, double value)
{
  auto it = double_properties.find(property_name);
  if (it == double_properties.end()) {
    return;
  }

  it->second = value;
}

void Configuration::set_string(std::string property_name, std::string value)
{
  auto it = string_properties.find(property_name);
  if (it == string_properties.end()) {
    return;
  }

  it->second = value;
}

std::string Configuration::get_enum_value(std::string property_name)
{
  auto it = enum_properties.find(property_name);
  if (it == enum_properties.end()) {
    return "";
  }

  return it->second.get_value();
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

std::vector<std::string> Configuration::get_enum_values(std::string property_name)
{
  auto it = enum_properties.find(property_name);
  if (it == enum_properties.end()) {
    return std::vector<std::string>();
  }

  return it->second.get_values();
}

std::vector<std::string> Configuration::get_enum_properties_names()
{
  std::vector<std::string> enum_properties_names;
  for (auto it = enum_properties.begin(); it != enum_properties.end(); ++it) {
    enum_properties_names.push_back(it->first);
  }
  return enum_properties_names;
}

std::vector<std::string> Configuration::get_boolean_properties_names()
{
  std::vector<std::string> boolean_properties_names;
  for (auto it = boolean_properties.begin(); it != boolean_properties.end(); ++it) {
    boolean_properties_names.push_back(it->first);
  }
  return boolean_properties_names;
}

std::vector<std::string> Configuration::get_integer_properties_names()
{
  std::vector<std::string> integer_properties_names;
  for (auto it = integer_properties.begin(); it != integer_properties.end(); ++it) {
    integer_properties_names.push_back(it->first);
  }
  return integer_properties_names;
}

std::vector<std::string> Configuration::get_double_properties_names()
{
  std::vector<std::string> double_properties_names;
  for (auto it = double_properties.begin(); it != double_properties.end(); ++it) {
    double_properties_names.push_back(it->first);
  }
  return double_properties_names;
}

std::vector<std::string> Configuration::get_string_properties_names()
{
  std::vector<std::string> string_properties_names;
  for (auto it = string_properties.begin(); it != string_properties.end(); ++it) {
    string_properties_names.push_back(it->first);
  }
  return string_properties_names;
}