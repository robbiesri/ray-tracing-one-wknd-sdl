#include "HittableList.h"

HittableList::HittableList(std::shared_ptr<Hittable> object) { Add(object); }

void HittableList::Clear() { m_objects.clear(); }

void HittableList::Add(std::shared_ptr<Hittable> object) {
  m_objects.push_back(object);
}

bool HittableList::Hit(const Ray& r, double tMin, double tMax,
  HitRecord& hr) const {
  HitRecord tempRec;
  bool hitAnything = false;
  auto closestSoFar = tMax;

  for (const auto &object : m_objects) {
    if (object->Hit(r, tMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      hr = tempRec;
    }
  }

  return hitAnything;
}