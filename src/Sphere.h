#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
  Sphere(const Point3 &center, double radius)
      : m_center(center), m_radius(radius) {}

  virtual bool Hit(const Ray &r, double tMin, double tMax,
                   HitRecord &hitRecord) const override;

private:
  Point3 m_center;
  double m_radius;
};