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
  // TODO normalize reflected direction?
  Vec3 reflected = Reflect(Normalize(inRay.Direction()), hitRecord.normal);
  reflected += m_fuzz * RandomInUnitSphere();

  scattered = Ray(hitRecord.hitPoint, reflected);
  attenuation = m_albedo;

  return (Dot(reflected, hitRecord.normal) > 0.0);
}