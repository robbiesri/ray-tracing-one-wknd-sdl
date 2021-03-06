#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "CPUImage.h"

#include <assert.h>
#include <cstdint>
#include <vector>

void uploadStagingImageToTexture(CPUImage &stagingImage,
                                 SDL_Texture *outputTexture) {

  SDL_Surface *outputSurface;
  SDL_LockTextureToSurface(outputTexture, NULL, &outputSurface);

  const SDL_PixelFormat *textureFormat = outputSurface->format;

  assert(stagingImage.m_width == outputSurface->w);
  assert(stagingImage.m_height == outputSurface->h);

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

  CPUImage stagingImage(imageWidth, imageHeight);
  for (uint32_t y = 0; y < imageHeight; y++) {
    for (uint32_t x = 0; x < imageWidth; x++) {
      Color3 &col = stagingImage(x, y);

      col[0] = double(x) / (imageWidth - 1);
      col[1] = double(y) / (imageHeight - 1);
      col[2] = 0.0f;
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

    uploadStagingImageToTexture(stagingImage, streamingTexture);

    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}