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

  Configuration& get_config() { return config; };

protected:
  Configuration config;
};

#endif