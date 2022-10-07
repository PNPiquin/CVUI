#ifndef IP_BASE_H
#define IP_BASE_H

#include <memory>

#include "utils/matrix.h"

template<typename T>
struct Pixel
{
  int x;
  int y;
  T value;
  Pixel()
  {
    x = 0;
    y = 0;
    value = 0;
  }

  Pixel(int i, int j, T v)
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
  void update_values(std::shared_ptr<Matrix<T>> img) { value = img->operator()(x, y); }
  void set_value(T value) { value = value; }
};

#endif