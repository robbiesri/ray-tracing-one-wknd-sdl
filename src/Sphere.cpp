#include "Sphere.h"

bool Sphere::Hit(const Ray &ray, double tMin, double tMax,
                 HitRecord &hitRecord) const {

  Vec3 scToRo = ray.origin() - m_center;
  auto b = dot(scToRo, ray.direction());
  auto c = dot(scToRo, scToRo) - (m_radius * m_radius);
  if ((c > 0.0) && (b > 0.0)) {
    return false;
  }

  double discriminant = (b * b) - c;
  if (discriminant < 0.0) {
    return false;
  }

  double t = -b - std::sqrt(discriminant);

  if (t < 0.0) {
    // This is inside sphere, so clamp to 0
    t = 0.0f;
  }

  if ((t < tMin) || (t > tMax)) {
    t = -b + std::sqrt(discriminant);
    if ((t < tMin) || (t > tMax)) {
      return false;
    }
  }

  hitRecord.t = t;
  hitRecord.hitPoint = ray.at(t);
  Vec3 outwardNormal = normalize(hitRecord.hitPoint - m_center);
  hitRecord.SetFaceNormal(ray, outwardNormal);

  return true;
}

#if 0
bool DoesRayHitSphere(const Point3 &sphereCenter, double sphereRadius,
               const Ray &ray) {
  // Normalized ray direction, a = 1
  Vec3 scToRo = ray.origin() - sphereCenter;
  auto c = dot(scToRo, scToRo) - (sphereRadius * sphereRadius);
  if (c < 0.0) {
    // Discriminant will be positive, giving a real root
    // Ray origin is inside sphere
    return true;
  }

  // We can factor two out from quadratic root equation
  auto b = dot(scToRo, ray.direction());
  if (b > 0.0) {
    // Ray starts outside sphere and points away.
    // The vector from ray to sphere is reversed for
    // scToRo, which is why we are testing for positive
    // instead of negative, the usual check for dot products
    return false;
  }

  double discriminant = (b * b) - c;
  return !(discriminant < 0.0);

  // Non-normalized ray direction
  // Vec3 scToRo = ray.origin() - sphereCenter;
  // auto a = dot(ray.direction(), ray.direction());
  // auto b = 2.0 * dot(scToRo, ray.direction());
  // auto c = dot(scToRo, scToRo) - (sphereRadius * sphereRadius);
  // auto discriminant = (b * b) - (4 * a * c);
  // return (discriminant > 0.0);
}

double RaySphereIntersection(const Point3 &sphereCenter, double sphereRadius,
                             const Ray &ray) {
  Vec3 scToRo = ray.origin() - sphereCenter;
  auto b = dot(scToRo, ray.direction());
  auto c = dot(scToRo, scToRo) - (sphereRadius * sphereRadius);
  if ((c > 0.0) && (b > 0.0)) {
    return -1.0;
  }

  double discriminant = (b * b) - c;
  if (discriminant < 0.0) {
    return -1.0;
  }

  double t = -b - std::sqrt(discriminant);

  if (t < 0.0) {
    // This is inside sphere, so clamp to 0 
    t = 0.0f;
  }

  return t;
}
#endif