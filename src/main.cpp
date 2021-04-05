#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "TraceEngine.h"

int main(int argc, char **argv) {

  SDL_SetMainReady();

  TraceEngine engine;

  if (engine.Init()) {
    engine.Run();
  }


  return 0;
}