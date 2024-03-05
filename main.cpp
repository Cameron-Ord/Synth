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

  InputMap *inputs = new InputMap;
  SynthWrapper *synth_functions = new SynthWrapper;
  Synth *synth = new Synth(inputs, synth_functions);

  delete inputs;
  delete synth_functions;
  delete synth;

  SDL_Quit();
  return 0;
}

void Synth::run_main_loop(InputMap *inputs, SynthWrapper *synfunc) {
  while (this->running == 1) {
    inputs->poll_events(this, synfunc);
    this->generate_samples(synfunc);
    this->do_render();
    SDL_Delay(16);
  }
}
