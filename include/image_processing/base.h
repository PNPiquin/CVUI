#ifndef IP_BASE_H
#define IP_BASE_H

#include <memory>

#include "utils/matrix.h"

struct Pixel
{
  int x;
  int y;
  int value;
  Pixel()
  {
    x = 0;
    y = 0;
    value = 0;
  }

  Pixel(int i, int j, int v)
  {
    x = i;
    y = j;
    value = v;
  }

  void set_coord(int xx, int yy)
  {
    x = xx;
    y = yy;
  }
  void update_values(std::shared_ptr<Matrix<uint8_t>> img) { value = img->operator()(x, y); }
  void set_value(int value) { value = value; }
};

#endif