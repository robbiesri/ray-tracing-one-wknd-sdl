#include "Camera.h"

Camera::Camera(Point3 lookFrom, Point3 lookAt, Vec3 upVec, double verticalFOV,
               double aspectRatio)
    : m_verticalFOV(verticalFOV), m_aspectRatio(aspectRatio) {
  double theta = DegreesToRadians(m_verticalFOV);
  double h = std::tan(theta / 2.0);

  m_viewportHeight = 2.0 * h;
  m_viewportWidth = m_viewportHeight * m_aspectRatio;

  // Why is this from - at? It _feels_ like this is the wrong direction
  Vec3 w = Normalize(lookFrom - lookAt);
  Vec3 u = Normalize(Cross(upVec, w));
  Vec3 v = Normalize(Cross(w, u));

  m_origin = lookFrom;
  m_horizontal = m_viewportWidth * u;
  m_vertical = m_viewportHeight * v;

  // m_upperLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) -
  //                    Vec3(0, 0, m_focalLength);
  // TODO - The focal length replacement needs tweaking (w)
  m_upperLeftCorner = m_origin - (m_horizontal / 2) - (m_vertical / 2) - w;
}

Ray Camera::GetRay(double s, double t) const {
  return Ray(m_origin, m_upperLeftCorner + (s * m_horizontal) +
                           (t * m_vertical) - m_origin);
}