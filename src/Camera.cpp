#include "Camera.h"

Camera::Camera(Point3 lookFrom, Point3 lookAt, Vec3 upVec, double verticalFOV,
               double aspectRatio, double aperture, double focusDistance)
    : m_verticalFOV(verticalFOV), m_aspectRatio(aspectRatio) {
  double theta = DegreesToRadians(m_verticalFOV);
  double h = std::tan(theta / 2.0);

  m_viewportHeight = 2.0 * h;
  m_viewportWidth = m_viewportHeight * m_aspectRatio;

  // Why is this from - at? It _feels_ like this is the wrong direction
  m_w = Normalize(lookFrom - lookAt);
  m_u = Normalize(Cross(upVec, m_w));
  m_v = Normalize(Cross(m_w, m_u));

  m_origin = lookFrom;
  m_horizontal = focusDistance * m_viewportWidth * m_u;
  m_vertical = focusDistance * m_viewportHeight * m_v;

  m_upperLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) - (focusDistance * m_w);

  m_lensRadius = aperture / 2.0;
}

Ray Camera::GetRay(double s, double t) const {
  Vec3 rd = m_lensRadius * RandomInUnitDisk();
  Vec3 offset = (m_u * rd.x()) + (m_v * rd.y());

  return Ray(m_origin + offset, m_upperLeftCorner + (s * m_horizontal) +
                                    (t * m_vertical) - m_origin - offset);
}