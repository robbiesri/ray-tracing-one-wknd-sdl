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

// Add multi-sampled version? Or have multi-sampled convertor?
struct SDL_PixelFormat;
uint32_t encodePixelIntoRGBA32(const Color3 &pixelRGB, uint32_t numSamples,
                               const SDL_PixelFormat &format);

