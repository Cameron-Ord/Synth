#include "inc/main.hpp"
#include <algorithm>

void Synth::synth_key_on(double freq, double *time) {
  /*
   making a pair from the freq, and the pointer to the time variable from the
   map. Any changes to the time variable will be reflected in the map it derives
   from.
  */
  std::pair<double, double *> freq_state_pair = std::make_pair(freq, time);
  playing_freqs->push_back(freq_state_pair);
}

void Synth::synth_key_off(double freq, double *time) {
  std::pair<double, double *> freq_state_pair = std::make_pair(freq, time);
  playing_freqs->erase(std::remove(playing_freqs->begin(), playing_freqs->end(),
                                   freq_state_pair),
                       playing_freqs->end());

  // Once the item has been removed from the vector, time gets reset to 0.0.
  *time = 0.0;
}

/*
 Using the keycode to find the respective frequency and time values
 also checks he held keys set. If the key is already in the set it just returns
 void.
*/
void Synth::keydown(int KEYCODE) {
  auto HELD_ITER = held_keys.find(KEYCODE);
  if (HELD_ITER != held_keys.end()) {
    return;
  }
  auto KM_ITER = frequencies->find(KEYCODE);
  /*
    If the key scancode is found, triggers the synth key on function and sends
    the repsective values. Note that the time variable is being sent as ref,
    this is so that the values actually change inside the map. Which is
    important for the key off code.
  */
  if (KM_ITER != frequencies->end()) {
    synth_key_on(KM_ITER->second.first, &KM_ITER->second.second);
    held_keys.insert(KEYCODE);
  }
}

// Pretty self explanatory.

void Synth::keyup(int KEYCODE) {
  auto held_it = held_keys.find(KEYCODE);
  if (held_it != held_keys.end()) {
    auto KM_ITER = frequencies->find(KEYCODE);
    if (KM_ITER != frequencies->end()) {
      synth_key_off(KM_ITER->second.first, &KM_ITER->second.second);
      held_keys.erase(KEYCODE);
    }
  }
}

// Handling key down and key up

void Synth::poll_events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
      keydown(e.key.keysym.sym);
      break;

    case SDL_KEYUP:
      keyup(e.key.keysym.sym);
      break;

    case SDL_QUIT:
      running = 0;
      break;
    }
  }
}
