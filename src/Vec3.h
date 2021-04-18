#pragma once

#include <array>
#include <cmath>
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

inline Vec3 operator/(const Vec3 &a, double c) {
  return (a * (1/c));
}

inline double Dot(const Vec3 &a, const Vec3 &b) {
  return (a.v[0] * b.v[0]) + (a.v[1] * b.v[1]) + (a.v[2] * b.v[2]);
}

inline Vec3 Cross(const Vec3 &a, const Vec3 &b) {
  return Vec3(
    (a.v[1]*b.v[2]) - (a.v[2] * b.v[1]),
    (a.v[2]*b.v[0]) - (a.v[0] * b.v[2]),
    (a.v[0]*b.v[1]) - (a.v[1] * b.v[0]));
}

inline Vec3 Normalize(Vec3 v) { return (v / v.Length()); }
