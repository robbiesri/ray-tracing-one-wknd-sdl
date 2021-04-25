#pragma once

#include "CoreUtils.h"
#include "Ray.h"

struct HitRecord;

class IMaterial {
public:
  virtual bool Scatter(const Ray &inRay, const HitRecord &hitRecord,
                       Color3 &attenuation, Ray &scattered) const = 0;
};

class LambertianMaterial : public IMaterial {
public:
  LambertianMaterial(const Color3 &a) : m_albedo(a) {}

  virtual bool Scatter(const Ray &inRay, const HitRecord &hitRecord,
                       Color3 &attenuation, Ray &scattered) const override;

// private:
public:
  Color3 m_albedo;
};

class MetalMaterial : public IMaterial {
public:
  MetalMaterial(const Color3 &a) : m_albedo(a) {}

  virtual bool Scatter(const Ray &inRay, const HitRecord &hitRecord,
                       Color3 &attenuation, Ray &scattered) const override;

private:
public:
  Color3 m_albedo;
};