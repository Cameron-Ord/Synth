#include "inc/main.hpp"
#include "inc/operations.hpp"
#include <SDL2/SDL_events.h>

void Synth::poll_events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
      key_down(e.key.keysym.scancode);
      break;
    case SDL_KEYUP:
      key_up(e.key.keysym.scancode);
      break;
    case SDL_QUIT:
      this->running = 0;
      break;
    }
  }
}

void Synth::key_down(int SCANCODE) {
  int found = find_key_pressed(this->KM, SCANCODE);
  if (found != -1) {
    this->playing[found] = 1;
  }
}

void Synth::key_up(int SCANCODE) {
  int found = find_key_pressed(this->KM, SCANCODE);
  if (found != -1) {
    this->playing[found] = 0;
  }
}
