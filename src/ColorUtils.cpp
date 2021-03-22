#include "ColorUtils.h"

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

uint32_t encodePixelIntoRGBA32(const Color3 &pixelRGB,
                               const SDL_PixelFormat &format) {

  // TODO: Clamp the colors!!

  uint8_t red = static_cast<uint8_t>(pixelRGB[0] * 255.0);
  uint8_t green = static_cast<uint8_t>(pixelRGB[1] * 255.0);
  uint8_t blue = static_cast<uint8_t>(pixelRGB[2] * 255.0);
  uint8_t alpha = 255;

  uint32_t encodedColor;
  encodedColor = (red << format.Rshift) & format.Rmask;
  encodedColor += (green << format.Gshift) & format.Gmask;
  encodedColor += (blue << format.Bshift) & format.Bmask;
  encodedColor += (alpha << format.Ashift) & format.Amask;

  return encodedColor;
}