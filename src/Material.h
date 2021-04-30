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
  MetalMaterial(const Color3 &a, double f) : m_albedo(a), m_fuzz(f < 1.0 ? f : 1.0) {}

  virtual bool Scatter(const Ray &inRay, const HitRecord &hitRecord,
                       Color3 &attenuation, Ray &scattered) const override;

//private:
public:
  Color3 m_albedo;
  double m_fuzz;
};

class DielectricMaterial : public IMaterial {
public:
  DielectricMaterial(double ior) : m_ior(ior) {}

  virtual bool Scatter(const Ray &inRay, const HitRecord &hitRecord,
                       Color3 &attenuation, Ray &scattered) const override;

      //private:
public:
  double m_ior;
};