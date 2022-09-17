#ifndef CONFIGURATION_PROCESSOR_H
#define CONFIGURATION_PROCESSOR_H

#include <memory>
#include <string>

#include "pipeline/configuration.h"
#include "pipeline/context.h"

class BaseProcessor
{
public:
  ~BaseProcessor() = default;
  virtual bool process(Context& context, std::string img_name, std::string output_img_name) { return true; };

  std::string get_processor_name() { return processor_name; };
  std::string get_processor_suffix() { return processor_suffix; };
  Configuration& get_config() { return config; };

protected:
  std::string processor_name;
  std::string processor_suffix;
  Configuration config;
};

#endif