#pragma once

#include "Vec3.h"

class Ray {
public:
  //Ray() {}
  Ray(const Point3 &origin, const Vec3 &direction)
      : m_origin(origin) {
    m_direction = Normalize(direction);
  }

  Point3 Origin() const { return m_origin; }
  Vec3 Direction() const { return m_direction; }

  Point3 At(double t) const { return m_origin + (t * m_direction); }

private:
  // public:
  Point3 m_origin;
  Vec3 m_direction;
};