#include "Vec3.h"

Vec3 Reflect(const Vec3 &v, const Vec3 &n) {
  // TODO Assert n is normalized
  return v - (2.0 * Dot(v, n) * n);
}

Vec3 Refract(const Vec3 &i, const Vec3 &n, double etaI, double etaT) {
  double cosTheta = std::fmin(Dot(-i, n), 1.0f);

  Vec3 refractPerp = (etaI / etaT) * (i + (cosTheta * n));

  // TODO Is it really correct to take the absolute value of the determinant?
  // What's the _actual_ path to take if parallel portion doesn't exist. It
  // seems like in this case, it's almost refracting entirely perpendicular to
  // the normal?
  Vec3 refractParallel =
      (-std::sqrt(std::fabs(1.0 - refractPerp.LengthSquared()))) * n;

  return (refractPerp + refractParallel);
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
Vec3 RefractAlt(const Vec3 &I, const Vec3 &N, double ior) {
  double cosi = Clamp(-1, 1, Dot(I, N));
  double etai = 1.0, etat = ior;
  Vec3 n = N;
  if (cosi < 0) {
    cosi = -cosi;
  } else {
    std::swap(etai, etat);
    n = -N;
  }
  double eta = etai / etat;
  double k = 1 - eta * eta * (1 - cosi * cosi);
  return k < 0 ? 0 : (eta * I) + ((eta * cosi - std::sqrt(k)) * n);
}

Vec3 RandomInUnitDisk() {
  while (true) {
    Vec3 p = Vec3(RandomDouble(-1.0, 1.0), RandomDouble(-1, 1), 0.0);
    if (p.LengthSquared() >= 1.0) {
      continue;
    }
    return p;
  }
}