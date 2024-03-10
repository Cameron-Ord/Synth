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
        double chorusd = chorus.tick(buffers.first[f], 0);
        double biquadd = biquad.tick(chorusd);
        buffers.second[f] = static_cast<int16_t>(biquadd);
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

/*
 I attempted to do a kaiser window here, but I think I'm doing something
 horribly wrong, maybe I'll read about it later, but it's a little beyond me at
 the moment. For now the hamming_window is easier and more straightforward.
*/

double *Synth::kaiser_window(double beta, int filter_length) {
  double *window = new double[filter_length];
  const double alpha = (filter_length - 1) / 2.0;
  for (int n = 0; n < filter_length; ++n) {
    window[n] = std::cyl_bessel_i(
                    0, beta * std::sqrt(1 - std::pow((n - alpha) / alpha, 2))) /
                std::cyl_bessel_i(0, beta);
  }
  return window;
}

double *Synth::generate_kcoefficients(int filter_length, double beta) {
  double *coeffs = new double[filter_length];
  double *kwincoeffs = kaiser_window(beta, filter_length);
  double cutoff = 0.85;

  for (int i = 0; i < filter_length; ++i) {
    if (i == (filter_length - 1) / 2) {
      coeffs[i] = 2 * cutoff;
    } else {
      double t = i - (static_cast<double>(filter_length) - 1) / 2;
      coeffs[i] = sin(2 * M_PI * cutoff * t) / (M_PI * t);
    }
    coeffs[i] *= kwincoeffs[i];
  }

  delete[] kwincoeffs;
  return coeffs;
}

/*
 hamming window sinc for generating the appropriate coefficients for the FIR
 filter. 0.54 and 0.46 are constants typically used in this.
*/

double *Synth::hamming_window(int filter_length) {
  double *window = new double[filter_length];
  for (int n = 0; n < filter_length; ++n) {
    window[n] = 0.54 - 0.46 * cos(2 * M_PI * n / (filter_length - 1));
  }
  return window;
}

double *Synth::generate_hcoefficients(int filter_length) {
  double *hcoeffs = new double[filter_length];
  double *hwincoeffs = hamming_window(filter_length);

  /*
   Here the nyquist frequency is used to generate a cutoff frequency in Hz, so
   that it can be used to generate the coefficients for the fir filter. I am
   going with 0.2 of the nyquist which is around 4000 hz, I can modify this to
   be a bit more consequential later, but this is fine for the moment.
  */

  double nyquist = static_cast<double>(SR) / 2.0;
  double cutoff = nyquist * 0.2;
  printf("SET CUTOFF : %6.3f\n", cutoff);
  int m = (filter_length - 1) / 2;
  for (int n = 0; n < filter_length; ++n) {
    if (n == m) {
      hcoeffs[n] = 2 * cutoff / SR;
    } else {
      hcoeffs[n] = sin(2 * M_PI * cutoff * (n - m) / SR) / (M_PI * (n - m));
    }
    hcoeffs[n] *= hwincoeffs[n];
  }
  delete[] hwincoeffs;
  return hcoeffs;
}

double Synth::handle_envelope_gen(double time) {
  if (time < AT) {
    double envelope = set_attack_env(time);
    return envelope;
  } else if (time < AT + DT) {
    double envelope = set_decay_env(time);
    return envelope;
  } else {
    double envelope = set_sustain_env();
    return envelope;
  }
  return 0.0;
}

double Synth::set_attack_env(double time) {
  double envelope = time / AT;
  return envelope;
}

double Synth::set_decay_env(double time) {
  double envelope = 1.0 - (1.0 - SL) * ((time - AT) / DT);
  return envelope;
}

double Synth::set_sustain_env() {
  double envelope = SL;
  return envelope;
}
