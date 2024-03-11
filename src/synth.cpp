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
        max = get_max_value(sample, max);
        buffers.first[i] = sample;
      }
    }

    for (int f = 0; f < BL; f++) {
      double sample = buffers.first[f];
      if (sample < 0.0 || sample > 0.0) {
        buffers.first[f] = normalize_sample(sample, max);
        buffers.first[f] *= INT16_MAX * 0.5;
        buffers.second[f] = static_cast<int16_t>(buffers.first[f]);
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

    double sawt_wave =
        saw_wave((*playing_freqs)[i].first, *(*playing_freqs)[i].second);

    double sawt_wave_mod = saw_wave((*playing_freqs)[i].first * (1.0 - 0.005),
                                    *(*playing_freqs)[i].second);
    double sawt_wave_mod_plus = saw_wave(
        (*playing_freqs)[i].first * (1.0 + 0.005), *(*playing_freqs)[i].second);

    double envelope = handle_envelope_gen(*(*playing_freqs)[i].second);
    sample += (((bit_crusher(sawt_wave, 8) + bit_crusher(sawt_wave_mod, 8) +
                 bit_crusher(sawt_wave_mod_plus, 8)) /
                3) *
               envelope);
    // Incrementing the time variable.
    *(*playing_freqs)[i].second += params["time_step"];
  }
  return sample;
}

double Synth::hard_clip(double sample, double t) {
  if (sample > t) {
    return t;
  } else if (sample < -t) {
    return -t;
  } else {
    return sample;
  }
}

double Synth::bit_crusher(double sample, int bit_depth) {
  double step_size = 1.0 / (pow(2, bit_depth) - 1);
  double quantized = round(sample / step_size) * step_size;
  return quantized;
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

double Synth::soft_clip(double sample, double amount) {
  return tanh(sample * amount);
}

double Synth::w(double freq) { return 2.0 * freq * M_PI; }

double Synth::saw_wave(double freq, double time) {
  return (2.0 / M_PI) * (freq * M_PI * fmod(time, 1.0 / freq) - (M_PI / 2.0));
}
double Synth::sine_wave(double freq, double time) {
  return sin(w(freq) * time);
}

double Synth::triangle_wave(double freq, double time) {
  return asin(sin(w(freq) * time)) * 2.0 / M_PI;
}

double Synth::square_wave(double freq, double time) {
  return sin(w(freq) * time) > 0.0 ? 1.0 : -1.0;
}
