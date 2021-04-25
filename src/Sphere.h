#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
  Sphere(const Point3 &center, double radius, std::shared_ptr<IMaterial> m)
      : m_center(center), m_radius(radius), matPtr(m) {}

  virtual bool Hit(const Ray &r, double tMin, double tMax,
                   HitRecord &hitRecord) const override;

private:
  Point3 m_center;
  double m_radius;
  std::shared_ptr<IMaterial> matPtr;
};