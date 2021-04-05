#include "TraceEngine.h"

#include "SDL.h"

#include "AssertUtils.h"
#include "CPUImage.h"
#include "CoreUtils.h"
#include "HittableList.h"
#include "Sphere.h"

#include <chrono>
#include <vector>

void UploadStagingImageToTexture(CPUImage &stagingImage,
                                 SDL_Texture *outputTexture) {
  auto startTime = std::chrono::system_clock::now();

  SDL_Surface *outputSurface;
  SDL_LockTextureToSurface(outputTexture, NULL, &outputSurface);

  const SDL_PixelFormat *textureFormat = outputSurface->format;

  WKND_ASSERT(stagingImage.m_width == outputSurface->w);
  WKND_ASSERT(stagingImage.m_height == outputSurface->h);

  uint32_t *outputTexData = static_cast<uint32_t *>(outputSurface->pixels);
  for (uint32_t rowIndex = 0; rowIndex < (uint32_t)outputSurface->h;
       rowIndex++) {
    uint32_t *outputRowAddress =
        outputTexData +
        ((rowIndex * outputSurface->pitch) / textureFormat->BytesPerPixel);
    for (uint32_t colIndex = 0; colIndex < (uint32_t)outputSurface->w;
         colIndex++) {
      uint32_t encodedColor = encodePixelIntoRGBA32(
          stagingImage(colIndex, rowIndex), *textureFormat);
      outputRowAddress[colIndex] = encodedColor;
    }
  }

  SDL_UnlockTexture(outputTexture);

  auto endTime = std::chrono::system_clock::now();
  auto duration = endTime - startTime;
  auto durationMilliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  std::cout << "Image upload time: " << durationMilliseconds << " ms" << '\n';
}

template <class T>
T lerp(double lerpVal, const T &a, const T &b) {
  return ((1.0 - lerpVal) * a) + (lerpVal * b);
}

Color3 RayColor(const Ray &r, const Hittable &world) {
  HitRecord hitRecord;
  if (world.Hit(r, 0.0, kInfinity, hitRecord)) {
    return (0.5 * (hitRecord.normal + Color3(1, 1, 1)));
  }

  // This code is based on the viewport vertical size, which isn't accessible
  // here Additionally, we aren't clamping the colors here...
  double lerpVal = 0.5 * (r.direction().y() + 1.0);
  return lerp(lerpVal, Color3(1.0), Color3(0.5, 0.7, 1.0));
}

void TraceEngine::GenerateImage(CPUImage &image) {

  // TODO: Put the scene somewhere
  HittableList worldList;
  worldList.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  worldList.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100.0));

  Vec3 horizontalViewportOffset = Vec3(m_viewportWidth, 0.0, 0.0);
  Vec3 verticalViewportOffset = Vec3(0.0, m_viewportHeight, 0.0);
  Vec3 upperLeftCorner = m_camOrigin - (horizontalViewportOffset / 2.0) -
                         (verticalViewportOffset / 2.0) -
                         Vec3(0., 0., m_focalLength);

  auto startTime = std::chrono::system_clock::now();
  const Vec3 baseRayDirection = upperLeftCorner - m_camOrigin;

  for (uint32_t y = 0; y < m_windowHeight; y++) {
    const double v = double(y) / (m_windowHeight - 1);
    const Vec3 verticalJitter = v * verticalViewportOffset;
    for (uint32_t x = 0; x < m_windowWidth; x++) {
      const double u = double(x) / (m_windowWidth - 1);
      const Vec3 horizontalJitter = u * horizontalViewportOffset;

      const Vec3 currentPixelJitter = horizontalJitter + verticalJitter;

      Ray currentRay = Ray(m_camOrigin, baseRayDirection + currentPixelJitter);

      // TODO: Function to map orientation of XY (Y starting at bottom and going
      // up) to target surface orientation (DX starts upper left, OGL lower
      // left).
      const uint32_t imageX = x;
      const uint32_t imageY = (m_windowHeight - 1) - y;
      image(imageX, imageY) = RayColor(currentRay, worldList);
    }
  }
  auto endTime = std::chrono::system_clock::now();
  auto duration = endTime - startTime;
  auto durationMilliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  std::cout << "Image generation time: " << durationMilliseconds << " ms"
            << '\n';
}

bool TraceEngine::Init() {

  SDL_Init(SDL_INIT_VIDEO);

  m_window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, m_windowWidth,
                              m_windowHeight, 0);
  if (!m_window) {
    return false;
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, 0);
  if (!m_renderer) {
    return false;
  }

  return true;
}

void TraceEngine::Run() {
  CPUImage stagingImage(m_windowWidth, m_windowHeight);

  // Is this the right pixel format?
  SDL_Texture *streamingTexture = SDL_CreateTexture(
      m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
      m_windowWidth, m_windowHeight);

  bool regenerateImage = true;

  SDL_Event sdlEvent;
  bool quitReceived = false;
  while (!quitReceived) {
    SDL_WaitEvent(&sdlEvent);

    switch (sdlEvent.type) {
    case SDL_QUIT:
      quitReceived = true;
      break;
    default:
      break;
    }

    if (regenerateImage) {
      GenerateImage(stagingImage);

      UploadStagingImageToTexture(stagingImage, streamingTexture);
      regenerateImage = false;
    }

    SDL_RenderCopy(m_renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
  }

  SDL_Quit();
}