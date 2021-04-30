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

// TODO: Move...somewhere?
static double Reflectance(double cosine, double refractionRatio) {
  // Schlick's approximation
  double r0 = (1.0 - refractionRatio) / (1.0 + refractionRatio);
  r0 = r0 * r0;
  return r0 + (1.0 - r0) * std::pow((1 - cosine), 5.0);
}

bool DielectricMaterial::Scatter(const Ray &inRay, const HitRecord &hitRecord,
                                 Color3 &attenuation, Ray &scattered) const {
  attenuation = Color3(1.0);
  
  double etaI = 1.0;
  double etaT = m_ior;
  if (!hitRecord.frontFace) {
    std::swap(etaI, etaT);
  }

  Vec3 normIn = Normalize(inRay.Direction());
  double cosTheta = std::fmin(Dot(-normIn, hitRecord.normal), 1.0);
  double sinTheta = std::sqrt(1.0 - (cosTheta * cosTheta));
  
  bool cannotRefract = ((etaI / etaT) * sinTheta) > 1.0;

  Vec3 direction;
  if (cannotRefract || (Reflectance(cosTheta, (etaI / etaT)) > RandomDouble())) {
    direction = Reflect(normIn, hitRecord.normal);
  } else {
    direction = Refract(normIn, hitRecord.normal, etaI, etaT);
  }

  scattered = Ray(hitRecord.hitPoint, direction);
  return true;
}