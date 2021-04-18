#include "Camera.h"

Camera::Camera(double aspectRatio) {
  m_aspectRatio = aspectRatio;
  m_viewportHeight = 2.0;
  m_viewportWidth = m_viewportHeight * m_aspectRatio;

  m_horizontal = Vec3(m_viewportWidth, 0., 0.);
  m_vertical = Vec3(0., m_viewportHeight, 0.);

  m_upperLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) -
                      Vec3(0, 0, m_focalLength);
}

Ray Camera::GetRay(double u, double v) const {
  return Ray(m_origin, m_upperLeftCorner + (u * m_horizontal) +
                           (v * m_vertical) - m_origin);
}