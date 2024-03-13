
#include "inc/main.hpp"
#include <cstdio>

void Inputs::poll_events(Synth* syn, Renderer* rend, SDL_State* init) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN: {
      keydown(e.key.keysym.sym, syn);
      break;
    }
    case SDL_KEYUP: {
      keyup(e.key.keysym.sym, syn);
      break;
    }

    case SDL_WINDOWEVENT: {
      switch (e.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED: {
        rend->apply_win_resize(init);
        break;
      }
      case SDL_WINDOWEVENT_RESIZED: {
        rend->apply_win_resize(init);
        break;
      }
      case SDL_WINDOWEVENT_HIDDEN: {
        break;
      }
      case SDL_WINDOWEVENT_SHOWN: {
        break;
      }
      }
      break;
    }
    case SDL_QUIT: {
      syn->set_run_state(0);
      break;
    }
    default:
      break;
    }
  }
}

void Inputs::keydown(int KEYCODE, Synth* syn) {
  auto HELD_ITER = held_keys.find(KEYCODE);
  if (HELD_ITER != held_keys.end()) {
    return;
  }
  std::map<int, Freq_Data>* f       = syn->get_freq_map();
  auto                      KM_ITER = f->find(KEYCODE);
  if (KM_ITER != f->end()) {
    synth_key_on(&KM_ITER->second);
    held_keys.insert(KEYCODE);
  }
}

void Inputs::keyup(int KEYCODE, Synth* syn) {
  auto HELD_ITER = held_keys.find(KEYCODE);
  if (HELD_ITER != held_keys.end()) {
    std::map<int, Freq_Data>* f       = syn->get_freq_map();
    auto                      KM_ITER = f->find(KEYCODE);
    if (KM_ITER != f->end()) {
      synth_key_off(&KM_ITER->second);
      held_keys.erase(KEYCODE);
    }
  }
}

void Inputs::synth_key_on(Freq_Data* fd) { fd->is_dead = 0; }

void Inputs::synth_key_off(Freq_Data* fd) { fd->is_dead = 1; }
