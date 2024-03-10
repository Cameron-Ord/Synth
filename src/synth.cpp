#include "inc/main.hpp"
#include <SDL2/SDL_events.h>
#include <cstdint>
#include <utility>
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
        double filtered = filter.tick(sample);
        max = get_max_value(filtered, max);
        buffers.first[i] = filtered;
      }
    }

    for (int f = 0; f < BL; f++) {
      double sample = buffers.first[f];
      if (sample < 0.0 || sample > 0.0) {
        buffers.first[f] = normalize_sample(sample, max);
        buffers.first[f] *= INT16_MAX * 0.5;
        double chorus_applied = chorus.tick(buffers.first[f], 0);
        buffers.second[f] = static_cast<int16_t>(chorus_applied);
      } else {
        buffers.second[f] = 0;
      }
    }
    buffer_enabled = 1;
  }
}

double Synth::generate_sample() {
  double sample = 0.0;
  for (size_t i = 0; i < playing_freqs->size(); i++) {
    // Generating a sine wave and adding it to the current sample.
    double wave =
        generate_wave((*playing_freqs)[i].first, *(*playing_freqs)[i].second);
    double envelope = handle_envelope_gen(*(*playing_freqs)[i].second);
    sample += (wave * envelope);
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
