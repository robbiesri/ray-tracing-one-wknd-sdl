#define SDL_MAIN_HANDLED
#include "SDL.h"

#include <cstdint>
#include <type_traits>
#include <vector>

#include <assert.h>

template <typename E> struct enable_bitmask_operators {
  static const bool enable = false;
};

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator|(E lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  return static_cast<E>(static_cast<underlying>(lhs) |
                        static_cast<underlying>(rhs));
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator&(E lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  return static_cast<E>(static_cast<underlying>(lhs) &
                        static_cast<underlying>(rhs));
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator^(E lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  return static_cast<E>(static_cast<underlying>(lhs) ^
                        static_cast<underlying>(rhs));
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator~(E lhs) {
  typedef typename std::underlying_type<E>::type underlying;
  return static_cast<E>(~static_cast<underlying>(lhs));
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator|=(E &lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  lhs = static_cast<E>(static_cast<underlying>(lhs) |
                       static_cast<underlying>(rhs));
  return lhs;
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator&=(E &lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  lhs = static_cast<E>(static_cast<underlying>(lhs) &
                       static_cast<underlying>(rhs));
  return lhs;
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator^=(E &lhs, E rhs) {
  typedef typename std::underlying_type<E>::type underlying;
  lhs = static_cast<E>(static_cast<underlying>(lhs) ^
                       static_cast<underlying>(rhs));
  return lhs;
}

enum class ColorBits : uint32_t {
  R = 1 << 0,
  G = 1 << 1,
  B = 1 << 2,
  A = 1 << 3,
};

template <> struct enable_bitmask_operators<ColorBits> {
  static const bool enable = true;
};

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

template <class ComponentFormatT> class CPUImage {
public:
  CPUImage(uint32_t width, uint32_t height, ColorBits colors)
      : m_width(width), m_height(height), m_colors(colors) {
    m_componentCount = CountColorBits(colors);
    m_rowStride = m_componentCount * m_width;

    m_imageData.resize(m_width * m_height * m_componentCount);
  }

  ComponentFormatT *getPixelData(uint32_t w, uint32_t h) {
    return &m_imageData[(m_rowStride * h) + w];
  }

  // TODO Add 2d operator()

  // private:
  std::vector<ComponentFormatT> m_imageData;
  uint32_t m_width;
  uint32_t m_height;
  ColorBits m_colors;
  uint32_t m_componentCount;
  uint32_t m_rowStride; // needed?
};

uint32_t encodePixelIntoRGBA32(const double *pixelRGB,
                               const SDL_PixelFormat *format) {

  uint8_t red = static_cast<uint8_t>(pixelRGB[0] * 255.0);
  uint8_t green = static_cast<uint8_t>(pixelRGB[1] * 255.0);
  uint8_t blue = static_cast<uint8_t>(pixelRGB[2] * 255.0);
  uint8_t alpha = 255;

  uint32_t encodedColor;
  encodedColor = (red << format->Rshift) & format->Rmask;
  encodedColor += (green << format->Gshift) & format->Gmask;
  encodedColor += (blue << format->Bshift) & format->Bmask;
  encodedColor += (alpha << format->Ashift) & format->Amask;

  return encodedColor;
}

template <class T>
void uploadStagingImageToTexture(const CPUImage<T> &stagingImage,
                                 SDL_Texture *outputTexture) {

  SDL_Surface *outputSurface;
  SDL_LockTextureToSurface(outputTexture, NULL, &outputSurface);

  const SDL_PixelFormat *textureFormat = outputSurface->format;

  assert(stagingImage.m_width == outputSurface->w);
  assert(stagingImage.m_height == outputSurface->h);
  assert(stagingImage.m_componentCount == 3); // TODO Derive from SDL_PixelFormat

  uint32_t *outputTexData = static_cast<uint32_t *>(outputSurface->pixels);
  for (uint32_t rowIndex = 0; rowIndex < (uint32_t)outputSurface->h;
       rowIndex++) {
    const uint32_t inputRowOffset = stagingImage.m_rowStride * rowIndex;
    uint32_t *outputRowAddress =
        outputTexData +
        ((rowIndex * outputSurface->pitch) / textureFormat->BytesPerPixel);
    for (uint32_t colIndex = 0; colIndex < (uint32_t)outputSurface->w;
         colIndex++) {
      const uint32_t inputOffsetIndex =
          inputRowOffset + (colIndex * stagingImage.m_componentCount);
      uint32_t encodedColor = encodePixelIntoRGBA32(&stagingImage.m_imageData[inputOffsetIndex], textureFormat);
      outputRowAddress[colIndex] = encodedColor;
    }
  }

  SDL_UnlockTexture(outputTexture);
}

int main(int argc, char **argv) {
  bool quitReceived = false;

  SDL_SetMainReady();
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window =
      SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  const uint32_t imageWidth = 1280;
  const uint32_t imageHeight = 720;

  CPUImage<double> stagingImage(imageWidth, imageHeight,
                                (ColorBits::R | ColorBits::G | ColorBits::B));
  for (uint32_t y = 0; y < imageHeight; y++) {
    const uint32_t rowOffset = stagingImage.m_rowStride * y;
    for (uint32_t x = 0; x < imageWidth; x++) {
      const uint32_t offsetIndex =
          rowOffset + (x * stagingImage.m_componentCount);

      stagingImage.m_imageData[offsetIndex + 0] = 0.0f;
      stagingImage.m_imageData[offsetIndex + 1] = 1.0f;
      stagingImage.m_imageData[offsetIndex + 2] = 0.0f;
    }
  }

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

    //uploadStagingImageToTexture(&stagingImage.m_imageData[0], streamingTexture);
    uploadStagingImageToTexture(stagingImage, streamingTexture);

    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}