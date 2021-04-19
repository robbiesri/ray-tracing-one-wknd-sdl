#include "ColorUtils.h"
#include "CoreUtils.h"

#include "SDL.h"

uint32_t CountColorBits(ColorBits color) {
  uint32_t count = 0;
  if (static_cast<uint32_t>(color & ColorBits::R) != 0) {
    count++;
  }
  if (static_cast<uint32_t>(color & ColorBits::G) != 0) {
    count++;
  }
  if (static_cast<uint32_t>(color & ColorBits::B) != 0) {
    count++;
  }
  if (static_cast<uint32_t>(color & ColorBits::A) != 0) {
    count++;
  }
  return count;
}

uint32_t encodePixelIntoRGBA32(const Color3 &pixelRGB, uint32_t numSamples,
                               const SDL_PixelFormat &format) {

  const double sampleScaler = 1.0 / numSamples;

  Color3 aaPixel = pixelRGB * sampleScaler;

  // Approximate gamma encoding
  aaPixel[0] = Clamp(sqrt(aaPixel[0]), 0.0, 1.0);
  aaPixel[1] = Clamp(sqrt(aaPixel[1]), 0.0, 1.0);
  aaPixel[2] = Clamp(sqrt(aaPixel[2]), 0.0, 1.0);

  uint8_t red = static_cast<uint8_t>(aaPixel[0] * 255.0);
  uint8_t green = static_cast<uint8_t>(aaPixel[1] * 255.0);
  uint8_t blue = static_cast<uint8_t>(aaPixel[2] * 255.0);
  uint8_t alpha = 255;

  uint32_t encodedColor;
  encodedColor = (red << format.Rshift) & format.Rmask;
  encodedColor += (green << format.Gshift) & format.Gmask;
  encodedColor += (blue << format.Bshift) & format.Bmask;
  encodedColor += (alpha << format.Ashift) & format.Amask;

  return encodedColor;
}