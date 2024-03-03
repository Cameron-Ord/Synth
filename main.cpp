#include "inc/main.hpp"

int init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_Log("Unable to init SDL2 %s", SDL_GetError());
    return -1;
  }
  return 0;
}

int main() {
  int err = init_sdl();
  if (err < 0) {
    return 1;
  }

  Synth *synth = new Synth;
  synth->run_main_loop();

  delete synth;
  SDL_Quit();
  return 0;
}

void Synth::run_main_loop() {
  while (this->running == 1) {
    this->poll_events();
    this->do_render();
    SDL_Delay(16);
  }
}
