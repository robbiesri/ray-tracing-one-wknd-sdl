#include <cstdint>

#define SDL_MAIN_HANDLED
#include "SDL.h"

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

void uploadStagingImageToTexture(const double *stagingImage,
                                 SDL_Texture *outputTexture) {

  SDL_Surface *outputSurface;
  SDL_LockTextureToSurface(outputTexture, NULL, &outputSurface);

  const SDL_PixelFormat *textureFormat = outputSurface->format;

  // assert staging image matches output surface
  // convert each pixel

  uint32_t *outputTexData = static_cast<uint32_t *>(outputSurface->pixels);
  for (uint32_t rowIndex = 0; rowIndex < (uint32_t)outputSurface->h;
       rowIndex++) {
    uint32_t *rowAddress = outputTexData + ((rowIndex * outputSurface->pitch) /
                                            textureFormat->BytesPerPixel);
    for (uint32_t colIndex = 0; colIndex < (uint32_t)outputSurface->w;
         colIndex++) {

      const size_t rowSize = 3 * outputSurface->w;
      uint32_t encodedColor = encodePixelIntoRGBA32(
          &stagingImage[rowIndex * rowSize + (colIndex * 3)], textureFormat);
      rowAddress[colIndex] = encodedColor;
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

  // Investigate which rendering backend is brought up
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  uint32_t imageWidth = 1280;
  uint32_t imageHeight = 720;

  double *stagingImage = new double[imageWidth * imageHeight * 3];

  for (uint32_t y = 0; y < imageHeight; y++) {
    for (uint32_t x = 0; x < imageWidth; x++) {
      uint32_t offsetIndex = 3 * ((y * imageWidth) + x);
      stagingImage[offsetIndex + 0] = 0.0f;
      stagingImage[offsetIndex + 1] = 1.0f;
      stagingImage[offsetIndex + 2] = 0.0f;
    }
  }

  //CPUImage<double> stagingImage(imageWidth, imageHeight, (ColorBits::R | ColorBits::G | ColorBits::B));

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

    // SDL_Surface *surfToUpdate;
    // SDL_LockTextureToSurface(streamingTexture, NULL, &surfToUpdate);

    // uint32_t *textureData = static_cast<uint32_t *>(surfToUpdate->pixels);

    // uint8_t red = 255;
    // uint8_t green = 0;
    // uint8_t blue = 0;
    // uint8_t alpha = 255;

    // const SDL_PixelFormat *textureFormat = surfToUpdate->format;

    // uint32_t encodedColor = 0;
    // encodedColor += (red << textureFormat->Rshift) & textureFormat->Rmask;
    // encodedColor += (green << textureFormat->Gshift) & textureFormat->Gmask;
    // encodedColor += (blue << textureFormat->Bshift) & textureFormat->Bmask;
    // encodedColor += (alpha << textureFormat->Ashift) & textureFormat->Amask;

    // for (uint32_t rowIndex = 0; rowIndex < (uint32_t)surfToUpdate->h;
    //     rowIndex++) {
    //  uint32_t *rowAddress = textureData + ((rowIndex * surfToUpdate->pitch) /
    //                                        textureFormat->BytesPerPixel);
    //  for (uint32_t colIndex = 0; colIndex < (uint32_t)surfToUpdate->w;
    //       colIndex++) {
    //    rowAddress[colIndex] = encodedColor;
    //  }
    //}

    // SDL_UnlockTexture(streamingTexture);

    uploadStagingImageToTexture(stagingImage, streamingTexture);

    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}