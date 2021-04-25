#include "Vec3.h"

Vec3 Reflect(const Vec3 &v, const Vec3 &n) {
  // TODO Assert n is normalized
  return v - (2.0 * Dot(v, n) * n);
}