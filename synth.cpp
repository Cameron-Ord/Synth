#include "inc/main.hpp"
#include <SDL2/SDL_events.h>
#include <algorithm>
#include <utility>

void Synth::handle_synth_key(double freq, int *playing) {
  std::pair<double, int> freq_state_pair;
  switch (*playing) {
  case 1:
    *playing = 0;
    freq_state_pair = std::make_pair(freq, *playing);
    playing_freqs.erase(std::remove(playing_freqs.begin(), playing_freqs.end(),
                                    freq_state_pair),
                        playing_freqs.end());
    break;
  case 0:
    *playing = 1;
    freq_state_pair = std::make_pair(freq, *playing);
    playing_freqs.push_back(freq_state_pair);
    break;
  }
}

void Synth::keydown(int SCANCODE) {
  auto HELD_ITER = held_keys.find(SCANCODE);
  if (HELD_ITER != held_keys.end()) {
    return;
  }
  auto KM_ITER = frequencies.find(SCANCODE);
  if (KM_ITER != frequencies.end()) {
    handle_synth_key(KM_ITER->second.first, &KM_ITER->second.second);
    held_keys.insert(SCANCODE);
  }
}

void Synth::keyup(int SCANCODE) {
  auto held_it = held_keys.find(SCANCODE);
  if (held_it != held_keys.end()) {
    auto KM_ITER = frequencies.find(SCANCODE);
    if (KM_ITER != frequencies.end()) {
      handle_synth_key(KM_ITER->second.first, &KM_ITER->second.second);
      held_keys.erase(SCANCODE);
    }
  }
}

void Synth::poll_events() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_KEYDOWN:
      keydown(e.key.keysym.scancode);
      break;

    case SDL_KEYUP:
      keyup(e.key.keysym.scancode);
      break;

    case SDL_QUIT:
      running = 0;
      break;
    }
  }
}
