#include "inc/main.hpp"
#include <SDL2/SDL_events.h>
#include <algorithm>
#include <cstdint>
#include <utility>

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

/*
  Generating the sample buffer.
*/

void Synth::create_sample_buffer() {
  if (!buffer_enabled) {
    double max = 0.0;
    for (int i = 0; i < BL; i++) {
      double sample = 0.0;
      buffers.first[i] = sample;
      sample = generate_sample();
      if (sample < 0.0 || sample > 0.0) {
        max = get_max_value(sample, max);
        buffers.first[i] = sample;
      }
    }

    for (int f = 0; f < BL; f++) {
      double sample = buffers.first[f];
      if (sample < 0.0 || sample > 0.0) {
        buffers.first[f] = normalize_sample(sample, max);
      }
      buffers.first[f] *= INT16_MAX;
      buffers.second[f] = static_cast<int16_t>(buffers.first[f]);
    }
    buffer_enabled = 1;
  }
}

double Synth::generate_sample() {
  double sample = 0.0;
  for (size_t i = 0; i < playing_freqs->size(); i++) {
    // Generating a sine wave and adding it to the current sample.
    sample +=
        generate_wave((*playing_freqs)[i].first, *(*playing_freqs)[i].second);

    // Incrementing the time variable.
    *(*playing_freqs)[i].second += params["time_step"];
  }
  return sample;
}

double Synth::w(double freq) { return 2.0 * freq * M_PI; }

double Synth::generate_wave(double freq, double time) {
  return (2.0 / M_PI) * (freq * M_PI * fmod(time, 1.0 / freq) - (M_PI / 2.0));
}

double Synth::get_max_value(double sample, double max) {
  double max_of_sample = fmax(max, fabs(sample));
  if (max_of_sample >= max) {
    max = max_of_sample;
  }
  return max;
}

double Synth::normalize_sample(double sample, double absmax) {
  return sample / absmax;
}
