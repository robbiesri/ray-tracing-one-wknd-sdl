#pragma once

#include "Ray.h"
#include "Vec3.h"

struct HitRecord {
  Point3 hitPoint;
  Vec3 normal;
  double t;
  bool frontFace;

  void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
    frontFace = Dot(ray.Direction(), outwardNormal) < 0.0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
public:
  virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hitRecord) const = 0;
};