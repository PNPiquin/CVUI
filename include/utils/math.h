#ifndef UTILS_MATH_H
#define UTILS_MATH_H

struct Point
{
  float x, y;

  Point(float _x, float _y);
  float distance_to(Point p);
};

struct Point3D
{
  float x, y, z;

  Point3D(float _x, float _y, float _z);
  float distance_to(Point3D p);

  Point3D& operator+=(const Point3D& rhs);
  Point3D operator+(const Point3D& rhs);
  Point3D& operator-=(const Point3D& rhs);
  Point3D operator-(const Point3D& rhs);
  Point3D& operator*=(const float rhs);
  Point3D operator*(const float rhs);
};

class NormalDistribution
{
public:
  NormalDistribution(float mu = 0.f, float sigma = 1.f);
  ~NormalDistribution() = default;

  float operator()(float value);

private:
  float mu, sigma;
};

#endif