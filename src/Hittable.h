#pragma once

#include "Ray.h"
#include "Vec3.h"

struct HitRecord {
  Point3 hitPoint;
  Vec3 normal;
  double t;
};

class Hittable {
public:
  virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord) const = 0;
};