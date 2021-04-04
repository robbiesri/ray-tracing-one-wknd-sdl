#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:

  HittableList() {}
  HittableList(std::shared_ptr<Hittable> object);

  void Clear();
  void Add(std::shared_ptr<Hittable> object);

  virtual bool Hit(const Ray &r, double tMin, double tMax, HitRecord &hr) const override;

//private:
public:
  std::vector<std::shared_ptr<Hittable>> m_objects;
};