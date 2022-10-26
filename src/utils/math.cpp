#include "utils/math.h"

#include <cmath>

#include "utils/constants.h"

Point::Point(float _x, float _y)
{
  x = _x;
  y = _y;
}

float Point::distance_to(Point p)
{
  return std::sqrt(std::pow(x - p.x, 2.f) + std::pow(y - p.y, 2.f));
}

Point3D::Point3D(float _x, float _y, float _z)
{
  x = _x;
  y = _y;
  z = _z;
}

float Point3D::distance_to(Point3D p)
{
  return std::sqrt(std::pow(x - p.x, 2.f) + std::pow(y - p.y, 2.f) + std::pow(z - p.z, 2.f));
}

Point3D& Point3D::operator+=(const Point3D& rhs)
{
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}
Point3D Point3D::operator+(const Point3D& rhs)
{
  return *this += rhs;
}

Point3D& Point3D::operator-=(const Point3D& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}
Point3D Point3D::operator-(const Point3D& rhs)
{
  return *this -= rhs;
}

Point3D& Point3D::operator*=(const float rhs)
{
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}
Point3D Point3D::operator*(const float rhs)
{
  return *this *= rhs;
}

NormalDistribution::NormalDistribution(float mu, float sigma)
  : mu(mu)
  , sigma(sigma)
{
}

float NormalDistribution::operator()(float value)
{
  return (1.f / (sigma * std::sqrt(2.f * M_PI))) * std::exp(-std::pow(value - mu, 2.f) / (2.f * std::pow(sigma, 2.f)));
}