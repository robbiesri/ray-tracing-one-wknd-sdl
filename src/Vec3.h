#pragma once

#include "CoreUtils.h"

#include <array>
#include <iostream>

// TODO Template types

class Vec3 {
public:
  Vec3() : v{0, 0, 0} {}
  Vec3(double s) : v{s, s, s} {}
  Vec3(double v0, double v1, double v2) : v{v0, v1, v2} {}

  double x() { return v[0]; }
  double y() { return v[1]; }
  double z() { return v[2]; }

  Vec3 operator-() const { return Vec3(-v[0], -v[1], -v[2]); }
  double operator[](int idx) const { return v[idx]; }
  double &operator[](int idx) { return v[idx]; }

  Vec3 &operator+=(const Vec3 &o) {
    v[0] += o.v[0];
    v[1] += o.v[1];
    v[2] += o.v[2];
    return *this;
  }

  Vec3 &operator*=(const double s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
    return *this;
  }

  Vec3 &operator/=(const double s) { return (*this *= 1 / s); }

  double LengthSquared() {
    return (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
  }

  double Length() { return std::sqrt(LengthSquared()); }

  inline static Vec3 Random() {
    return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
  }

  inline static Vec3 Random(double min, double max) {
    return Vec3(RandomDouble(min, max), RandomDouble(min, max),
                RandomDouble(min, max));
  }

  inline bool NearZero() const {

    // TODO Pick an actually significant epsilon
    const double kSmall = 1e-8;
    return (std::fabs(v[0]) < kSmall) && (std::fabs(v[1]) < kSmall) &&
           (std::fabs(v[2]) < kSmall);
  }

public:
  std::array<double, 3> v;
};

using Point3 = Vec3;
using Color3 = Vec3;

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v3) {
  return (out << v3.v[0] << ' ' << v3.v[1] << ' ' << v3.v[2]);
}

inline Vec3 operator+(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]);
}

inline Vec3 operator-(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]);
}

inline Vec3 operator*(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2]);
}

inline Vec3 operator*(const Vec3 &a, double c) {
  return Vec3(a.v[0] * c, a.v[1] * c, a.v[2] * c);
}

inline Vec3 operator*(double c, const Vec3 &a) {
  return Vec3(a.v[0] * c, a.v[1] * c, a.v[2] * c);
}

inline Vec3 operator/(const Vec3 &a, double c) { return (a * (1 / c)); }

inline double Dot(const Vec3 &a, const Vec3 &b) {
  return (a.v[0] * b.v[0]) + (a.v[1] * b.v[1]) + (a.v[2] * b.v[2]);
}

inline Vec3 Cross(const Vec3 &a, const Vec3 &b) {
  return Vec3((a.v[1] * b.v[2]) - (a.v[2] * b.v[1]),
              (a.v[2] * b.v[0]) - (a.v[0] * b.v[2]),
              (a.v[0] * b.v[1]) - (a.v[1] * b.v[0]));
}

inline Vec3 Normalize(Vec3 v) { return (v / v.Length()); }

inline Vec3 RandomInUnitSphere() {
  while (true) {
    Vec3 point = Vec3::Random(-1.0, 1.0);
    if (point.LengthSquared() >= 1.0) {
      continue;
    }
    return point;
  }
}

inline Vec3 RandomUnitVector() { return Normalize(RandomInUnitSphere()); }

inline Vec3 RandomInHemisphere(const Vec3 &normal) {
  Vec3 inUnitSphere = RandomInUnitSphere();
  if (Dot(inUnitSphere, normal) > 0.0) {
    return inUnitSphere;
  } else {
    return (-inUnitSphere);
  }
}

Vec3 Reflect(const Vec3 &v, const Vec3 &n);

Vec3 Refract(const Vec3 &i, const Vec3 &n, double etaI, double etaT);
Vec3 RefractAlt(const Vec3 &I, const Vec3 &N, double ior);

Vec3 RandomInUnitDisk();