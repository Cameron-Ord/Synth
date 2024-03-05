#include "inc/main.hpp"
#include <SDL2/SDL_events.h>

void InputMap::synth_key_pressed(int index, int playing[]) {
  playing[index] = 1;
}

void InputMap::synth_key_released(int index, int playing[]) {
  playing[index] = 0;
}

void InputMap::ctrls_key_pressed(int SCANCODE, int *wave_ptr_index) {
  switch (SCANCODE) {
  case LARROW:
    this->previous_wave_fn(wave_ptr_index);
    break;
  case RARROW:
    this->next_wave_fn(wave_ptr_index);
    break;
  }
}

void InputMap::previous_wave_fn(int *wave_ptr_index) {
  int LEN = 6;
  int index_cpy = *wave_ptr_index;
  index_cpy--;
  if (index_cpy < 0) {
    index_cpy = LEN - 1;
  }
  *wave_ptr_index = index_cpy;
}

void InputMap::next_wave_fn(int *wave_ptr_index) {
  int LEN = 6;
  int index_cpy = *wave_ptr_index;
  index_cpy++;
  if (index_cpy > LEN - 1) {
    index_cpy = 0;
  }
  *wave_ptr_index = index_cpy;
}

void InputMap::poll_events(Synth *syn, SynthWrapper *synfunc) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
      this->key_down(e.key.keysym.scancode, syn->playing,
                     &synfunc->wave_ptr_index);
      break;
    case SDL_KEYUP:
      this->key_up(e.key.keysym.scancode, syn->playing);
      break;
    case SDL_QUIT:
      syn->running = 0;
      break;
    }
  }
}

void InputMap::key_down(int SCANCODE, int playing[], int *wave_ptr_index) {
  auto iteration = this->KM->find(SCANCODE);
  if (iteration != this->KM->end()) {
    switch (iteration->second.second) {
    case 2:
      this->ctrls_key_pressed(iteration->first, wave_ptr_index);
      break;
    case 1:
      this->synth_key_pressed(iteration->second.first, playing);
      break;
    }
  }
}

void InputMap::key_up(int SCANCODE, int playing[]) {
  auto iteration = this->KM->find(SCANCODE);
  if (iteration != this->KM->end()) {
    switch (iteration->second.second) {
    case 1:
      this->synth_key_released(iteration->second.first, playing);
      break;
    }
  }
}
