#pragma once

#include "CoreUtils.h"

class Camera {
public:
  Camera(double aspectRatio);

  Ray GetRay(double u, double v) const;

private:
  double m_aspectRatio;
  double m_viewportHeight;
  double m_viewportWidth;
  double m_focalLength = 1.0;

  Point3 m_origin = Point3(0.0);
  //Point3 m_lowerLeftCorner;
  Point3 m_upperLeftCorner;

  Vec3 m_horizontal;
  Vec3 m_vertical;
};