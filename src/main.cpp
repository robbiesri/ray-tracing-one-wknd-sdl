#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "AssertUtils.h"
#include "CPUImage.h"
#include "Ray.h"

#include <chrono>
#include <cstdint>
#include <vector>

void UploadStagingImageToTexture(CPUImage &stagingImage,
                                 SDL_Texture *outputTexture) {

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
}

template <class T>
T lerp(double lerpVal, const T &a, const T &b) {
  return ((1.0 - lerpVal) * a) + (lerpVal * b);
}

bool HitSphere(const Point3 &sphereCenter, double sphereRadius,
               const Ray &ray) {
  // Normalized ray direction, a = 1
  Vec3 scToRo = ray.origin() - sphereCenter;
  auto c = dot(scToRo, scToRo) - (sphereRadius * sphereRadius);
  if (c < 0.0) {
    // Discriminant will be positive, giving a real root
    // Ray origin is inside sphere
    return true;
  }

  // We can factor two out from quadratic root equation
  auto b = dot(scToRo, ray.direction());
  if (b > 0.0) {
    // Ray starts outside sphere and points away.
    // The vector from ray to sphere is reversed for
    // scToRo, which is why we are testing for positive
    // instead of negative, the usual check for dot products
    return false;
  }

  double discriminant = (b * b) - c;
  return !(discriminant < 0.0);

  // Non-normalized ray direction
  // Vec3 scToRo = ray.origin() - sphereCenter;
  // auto a = dot(ray.direction(), ray.direction());
  // auto b = 2.0 * dot(scToRo, ray.direction());
  // auto c = dot(scToRo, scToRo) - (sphereRadius * sphereRadius);
  // auto discriminant = (b * b) - (4 * a * c);
  // return (discriminant > 0.0);
}

Color3 RayColor(const Ray &r) {
  if (HitSphere(Point3(0, 0, -1), 0.5, r)) {
    return Color3(1, 0, 0);
  }
  double lerpVal = 0.5 * (r.direction().y() + 1.0);
  return lerp(lerpVal, Color3(1.0), Color3(0.5, 0.7, 1.0));
}

int main(int argc, char **argv) {
  bool quitReceived = false;

  // Image info
  const uint32_t imageWidth = 1280;
  const uint32_t imageHeight = 720;
  const double aspectRatio = double(imageWidth) / imageHeight;

  // Camera info
  double viewportHeight = 2.0f;
  double viewportWidth = viewportHeight * aspectRatio;
  double focalLength = 1.0f;

  Point3 camOrigin = Point3(0.0);
  Vec3 horizontalViewportOffset = Vec3(viewportWidth, 0.0, 0.0);
  Vec3 verticalViewportOffset = Vec3(0.0, viewportHeight, 0.0);
  Vec3 upperLeftCorner = camOrigin - (horizontalViewportOffset / 2.0) -
                         (verticalViewportOffset / 2.0) -
                         Vec3(0., 0., focalLength);

  SDL_SetMainReady();
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, imageWidth, imageHeight, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  CPUImage stagingImage(imageWidth, imageHeight);

  auto startTime = std::chrono::system_clock::now();
  const Vec3 baseRayDirection = upperLeftCorner - camOrigin;

  for (uint32_t y = 0; y < imageHeight; y++) {
    const double v = double(y) / (imageHeight - 1);
    const Vec3 verticalJitter = v * verticalViewportOffset;
    for (uint32_t x = 0; x < imageWidth; x++) {
      const double u = double(x) / (imageWidth - 1);
      const Vec3 horizontalJitter = u * horizontalViewportOffset;

      const Vec3 currentPixelJitter = horizontalJitter + verticalJitter;

      Ray currentRay = Ray(camOrigin, baseRayDirection + currentPixelJitter);

      // TODO: Function to map orientation of XY (Y starting at bottom and going up)
      // to target surface orientation (DX starts upper left, OGL lower left).
      const uint32_t imageX = x;
      const uint32_t imageY = (imageHeight - 1) - y;
      stagingImage(imageX, imageY) = RayColor(currentRay);
    }
  }
  auto endTime = std::chrono::system_clock::now();
  auto duration = endTime - startTime;
  auto durationMilliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  std::cout << "Image generation time: " << durationMilliseconds << " ms"
            << '\n';

  // Is this the right pixel format?
  SDL_Texture *streamingTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_STREAMING, imageWidth, imageHeight);

  SDL_Event sdlEvent;
  while (!quitReceived) {
    SDL_WaitEvent(&sdlEvent);

    switch (sdlEvent.type) {
    case SDL_QUIT:
      quitReceived = true;
      break;
    default:
      break;
    }

    UploadStagingImageToTexture(stagingImage, streamingTexture);

    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}