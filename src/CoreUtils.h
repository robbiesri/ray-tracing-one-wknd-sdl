#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <memory>

const double kInfinity = std::numeric_limits<double>::infinity();
const double kPi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) {
  return (degrees * kPi) / 180.0;
}

#include "Ray.h"