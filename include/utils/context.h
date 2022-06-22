#ifndef CONTEXT_H
#define CONTEXT_H

#include <cstdint>
#include <string>
#include <utils/contextbase.h>

class GrayContext : public Context<uint8_t>
{
public:
  void process_kmeans(std::string img_name);
  void generate_border_image(std::string img_name);
  void save_image(std::string img_name);
};

class RGBAContext : public Context<uint32_t>
{
public:
  void save_image(std::string img_name);
};

#endif