#include "CoreUtils.h"

#include <random>

static std::mt19937 s_generator;

inline double RandomDouble() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(s_generator);
}

inline double RandomDouble(double min, double max) {
  static std::uniform_real_distribution<double> distribution(min, max);
  return distribution(s_generator);
}

inline double Clamp(double x, double min, double max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}