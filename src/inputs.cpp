
#include "inc/main.hpp"
#include <SDL2/SDL_keycode.h>
#include <cstdio>

void Inputs::poll_events(Synth* syn, Renderer* rend, SDL_State* init) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN: {
      if (e.key.keysym.sym == SDLK_ESCAPE) {
        clear_chord_pipe(syn);
      }
      if (SDL_GetModState() & KMOD_SHIFT) {
        chord_keydown(e.key.keysym.sym, syn);
      } else {
        keydown(e.key.keysym.sym, syn);
      }
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

void Inputs::clear_chord_pipe(Synth* syn) {
  std::map<int, Freq_Data>* cf = syn->get_cfreq_map();
  for (auto& pair : *cf) {
    Freq_Data* fptr = &pair.second;
    if (!fptr->is_dead) {
      fptr->is_dead = 1;
    }
  }
  held_chord_keys.clear();
}

void Inputs::chord_keydown(int KEYCODE, Synth* syn) {
  auto HELD_ITER = held_chord_keys.find(KEYCODE);
  if (HELD_ITER != held_chord_keys.end()) {
    chord_key_off(KEYCODE, syn);
    held_chord_keys.erase(KEYCODE);
    return;
  }

  std::map<int, Freq_Data>* cf      = syn->get_cfreq_map();
  auto                      KM_ITER = cf->find(KEYCODE);
  if (KM_ITER != cf->end()) {
    chord_key_on(&KM_ITER->second);
    held_chord_keys.insert(KEYCODE);
  }
}

void Inputs::chord_key_on(Freq_Data* cfd) { cfd->is_dead = 0; }

void Inputs::chord_key_off(int KEYCODE, Synth* syn) {
  std::map<int, Freq_Data>* cf      = syn->get_cfreq_map();
  auto                      KM_ITER = cf->find(KEYCODE);
  if (KM_ITER != cf->end()) {
    KM_ITER->second.is_dead = 1;
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

void Inputs::synth_key_off(Freq_Data* fd) {
  fd->is_dead = 1;
  fd->time    = 0.0;
}
