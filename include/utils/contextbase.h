#ifndef CONTEXT_BASE_H
#define CONTEXT_BASE_H

#include <map>
#include <memory>
#include <string>

#include "utils/matrix.h"

template<typename T>
class Context
{
private:
  std::map<std::string, std::shared_ptr<Matrix<T>>> imgs;

public:
  Context();
  ~Context() = default;

  void add_image(std::string img_name, std::shared_ptr<Matrix<T>> img);
  std::shared_ptr<Matrix<T>> get_image(std::string img_name);
};

template<typename T>
Context<T>::Context()
{
}

template<typename T>
void Context<T>::add_image(std::string img_name, std::shared_ptr<Matrix<T>> img)
{
  imgs.insert({ img_name, img });
}

template<typename T>
std::shared_ptr<Matrix<T>> Context<T>::get_image(std::string img_name)
{
  auto img = imgs.find(img_name);
  if (img == imgs.end()) {
    return std::shared_ptr<Matrix<T>>();
  }
  return img->second;
}

#endif