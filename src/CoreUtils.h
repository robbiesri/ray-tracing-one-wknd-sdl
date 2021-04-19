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

//inline double random_double() {
//  // Returns a random real in [0,1).
//  return std::rand() / (RAND_MAX + 1.0);
//}
//
//inline double random_double(double min, double max) {
//  // Returns a random real in [min,max).
//  return min + (max - min) * random_double();
//}

double RandomDouble();
double RandomDouble(double min, double max);

double Clamp(double x, double min, double max);
