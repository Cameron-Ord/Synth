#include "inc/main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <algorithm>

void Inputs::synth_key_on(double freq, double* time,
                          std::vector<std::pair<double, double*>>* playing_freqs) {
  /*
   making a pair from the freq, and the pointer to the time variable from the
   map. Any changes to the time variable will be reflected in the map it derives
   from.
  */
  std::pair<double, double*> freq_state_pair = std::make_pair(freq, time);
  playing_freqs->push_back(freq_state_pair);
}

void Inputs::synth_key_off(double freq, double* time,
                           std::vector<std::pair<double, double*>>* playing_freqs) {

  std::pair<double, double*> freq_state_pair = std::make_pair(freq, time);
  playing_freqs->erase(std::remove(playing_freqs->begin(), playing_freqs->end(), freq_state_pair),
                       playing_freqs->end());

  // Once the item has been removed from the vector, time gets reset to 0.0.
  *time = 0.0;
}

/*
 Using the keycode to find the respective frequency and time values
 also checks he held keys set. If the key is already in the set it just returns
 void.
*/
void Inputs::keydown(int KEYCODE, Synth* syn) {
  auto HELD_ITER = held_keys.find(KEYCODE);
  if (HELD_ITER != held_keys.end()) {
    return;
  }

  std::map<int, std::pair<double, double>>* f       = syn->get_freqs_map();
  auto                                      KM_ITER = f->find(KEYCODE);
  /*
    If the key scancode is found, triggers the synth key on function and sends
    the repsective values. Note that the time variable is being sent as ref,
    this is so that the values actually change inside the map. Which is
    important for the key off code.
  */
  if (KM_ITER != f->end()) {
    synth_key_on(KM_ITER->second.first, &KM_ITER->second.second, syn->get_playing_freqs());
    held_keys.insert(KEYCODE);
  }
}

// Pretty self explanatory.

void Inputs::keyup(int KEYCODE, Synth* syn) {
  auto held_it = held_keys.find(KEYCODE);
  if (held_it != held_keys.end()) {

    std::map<int, std::pair<double, double>>* f       = syn->get_freqs_map();
    auto                                      KM_ITER = f->find(KEYCODE);
    if (KM_ITER != f->end()) {
      synth_key_off(KM_ITER->second.first, &KM_ITER->second.second, syn->get_playing_freqs());
      held_keys.erase(KEYCODE);
    }
  }
}

// Handling key down and key up

void Inputs::poll_events(Synth* syn, Renderer* rend, Initializer* init) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
      keydown(e.key.keysym.sym, syn);
      break;

    case SDL_KEYUP:
      keyup(e.key.keysym.sym, syn);
      break;

    case SDL_WINDOWEVENT:
      switch (e.window.event) {
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        rend->apply_win_resize(init);
        break;
      case SDL_WINDOWEVENT_RESIZED:
        rend->apply_win_resize(init);
        break;
      case SDL_WINDOWEVENT_HIDDEN:
        break;
      case SDL_WINDOWEVENT_SHOWN:
        break;
      }
      break;

    case SDL_QUIT:
      syn->set_run_state(0);
      break;
    default:
      break;
    }
  }
}
