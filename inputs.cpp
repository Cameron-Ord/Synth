#include "inc/main.hpp"
#include "inc/operations.hpp"
#include <SDL2/SDL_events.h>

void Synth::synth_key_pressed(int index) { this->playing[index] = 1; }

void Synth::synth_key_released(int index) { this->playing[index] = 0; }

void Synth::ctrls_key_pressed(int SCANCODE) {
  switch (SCANCODE) {
  case LARROW:
    this->previous_wave_fn();
    break;
  case RARROW:
    this->next_wave_fn();
    break;
  }
}

void Synth::previous_wave_fn() {
  int LEN = 6;
  int index_cpy = this->wave_ptr_index;
  index_cpy--;
  if (index_cpy < 0) {
    index_cpy = LEN - 1;
  }
  this->wave_ptr_index = index_cpy;
}

void Synth::next_wave_fn() {
  int LEN = 6;
  int index_cpy = this->wave_ptr_index;
  index_cpy++;
  if (index_cpy > LEN - 1) {
    index_cpy = 0;
  }
  this->wave_ptr_index = index_cpy;
}

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
  auto iteration = this->KM->find(SCANCODE);
  if (iteration != this->KM->end()) {
    switch (iteration->second.second) {
    case 2:
      this->ctrls_key_pressed(iteration->first);
      break;
    case 1:
      this->synth_key_pressed(iteration->second.first);
      break;
    }
  }
}

void Synth::key_up(int SCANCODE) {
  auto iteration = this->KM->find(SCANCODE);
  if (iteration != this->KM->end()) {
    switch (iteration->second.second) {
    case 1:
      this->synth_key_released(iteration->second.first);
      break;
    }
  }
}
