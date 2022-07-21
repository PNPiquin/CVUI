#ifndef CONFIGURATION_PROCESSOR_H
#define CONFIGURATION_PROCESSOR_H

#include <memory>

#include "pipeline/configuration.h"

class BaseProcessor
{
public:
  BaseProcessor();
  ~BaseProcessor() = default;

  virtual bool process() = 0;

private:
  Configuration config;
};

#endif