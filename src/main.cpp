#include <cstdint>

#define SDL_MAIN_HANDLED
#include "SDL.h"

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

    SDL_Surface *surfToUpdate;
    SDL_LockTextureToSurface(streamingTexture, NULL, &surfToUpdate);

    uint32_t *textureData = static_cast<uint32_t *>(surfToUpdate->pixels);

    uint8_t red = 255;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t alpha = 255;

    const SDL_PixelFormat *textureFormat = surfToUpdate->format;

    uint32_t encodedColor = 0;
    encodedColor += (red << textureFormat->Rshift) & textureFormat->Rmask;
    encodedColor += (green << textureFormat->Gshift) & textureFormat->Gmask;
    encodedColor += (blue << textureFormat->Bshift) & textureFormat->Bmask;
    encodedColor += (alpha << textureFormat->Ashift) & textureFormat->Amask;

    for (uint32_t rowIndex = 0; rowIndex < (uint32_t)surfToUpdate->h;
         rowIndex++) {
      uint32_t *rowAddress = textureData + ((rowIndex * surfToUpdate->pitch) /
                                            textureFormat->BytesPerPixel);
      for (uint32_t colIndex = 0; colIndex < (uint32_t)surfToUpdate->w;
           colIndex++) {
        rowAddress[colIndex] = encodedColor;
      }
    }

    SDL_UnlockTexture(streamingTexture);

    SDL_RenderCopy(renderer, streamingTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}