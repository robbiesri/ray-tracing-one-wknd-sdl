#pragma once

#include "CoreUtils.h"
#include "Camera.h"

struct SDL_Window;
struct SDL_Renderer;
class CPUImage;

class TraceEngine {
public:
  TraceEngine() : m_camera(double(kDefaultWindowWidth) / kDefaultWindowHeight) {};

  bool Init();

  void Run();

protected:
  void GenerateImage(CPUImage& image);

private:
  static constexpr int32_t kTraceDepth = 50;
  static constexpr uint32_t kDefaultWindowWidth = 640;
  static constexpr uint32_t kDefaultWindowHeight = 360;

  uint32_t m_windowWidth = kDefaultWindowWidth;
  uint32_t m_windowHeight = kDefaultWindowHeight;
  //double m_aspectRatio = double(kDefaultWindowWidth) / kDefaultWindowHeight;

  // TODO: Separate image size

  // Camera info
  // Viewport size + lens distance from view point
  //double m_viewportHeight = 2.0;
  //double m_viewportWidth = m_viewportHeight * m_aspectRatio;
  //double m_focalLength = 1.0;
  //
  //Point3 m_camOrigin = Point3(0.0);
  Camera m_camera;

  // SDL interface
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;

};