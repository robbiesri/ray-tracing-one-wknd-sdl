#pragma once

#include "TypedBitmask.h"
#include "Vec3.h"

enum class ColorBits : uint32_t {
  R = 1 << 0,
  G = 1 << 1,
  B = 1 << 2,
  A = 1 << 3,
};

template <> struct enable_bitmask_operators<ColorBits> {
  static const bool enable = true;
};

uint32_t CountColorBits(ColorBits color);

struct SDL_PixelFormat;
uint32_t encodePixelIntoRGBA32(const Color3 &pixelRGB,
                               const SDL_PixelFormat &format);
