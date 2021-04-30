#pragma once

#include "CoreUtils.h"
#include "Ray.h"

class Camera {
public:
  Camera() = default;
  Camera(Point3 lookFrom, Point3 lookAt, Vec3 upVec, double verticalFOV, double aspectRatio, double aperture, double focusDistance);

  Ray GetRay(double s, double t) const;

private:
  double m_verticalFOV;
  double m_aspectRatio;
  double m_viewportHeight;
  double m_viewportWidth;
  //double m_focalLength = 1.0;

  Point3 m_origin = Point3(0.0);
  //Point3 m_lowerLeftCorner;
  Point3 m_upperLeftCorner;

  Vec3 m_horizontal;
  Vec3 m_vertical;

  Vec3 m_u, m_v, m_w;
  double m_lensRadius;
};