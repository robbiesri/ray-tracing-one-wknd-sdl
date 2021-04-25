#include "Material.h"

#include "Hittable.h"

#define USE_CORRECTED_LAMBERTIAN

bool LambertianMaterial::Scatter(const Ray &inRay, const HitRecord &hitRecord,
                                 Color3 &attenuation, Ray &scattered) const {

#if defined(USE_CORRECTED_LAMBERTIAN)
  Point3 scatterDirection =
      RandomInHemisphere(hitRecord.normal); // hemispherical distribution
#else
  Point3 scatterDirection = hitRecord.normal + RandomUnitVector(); // incorrect
  if (scatterDirection.NearZero()) {
    scatterDirection = hitRecord.normal;
  }
#endif

  scattered = Ray(hitRecord.hitPoint, scatterDirection);
  attenuation = m_albedo;

  return true;
}

bool MetalMaterial::Scatter(const Ray &inRay, const HitRecord &hitRecord,
                                 Color3 &attenuation, Ray &scattered) const {
  Vec3 reflected = Reflect(Normalize(inRay.Direction()), hitRecord.normal);
  // TODO normalize reflected direction?

  scattered = Ray(hitRecord.hitPoint, reflected);
  attenuation = m_albedo;

  return (Dot(reflected, hitRecord.normal) > 0.0);
}