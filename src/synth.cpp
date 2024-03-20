#include "inc/main.hpp"

void Synth::create_sample_buffer() {
  if (!buffer_enabled) {
    double max = 0.0;
    for (int i = 0; i < BL; i++) {
      double sample = 0.0;
      fbuffer[i]    = sample;
      sample        = generate_sample();
      if (sample < 0.0 || sample > 0.0) {
        max        = get_max_value(sample, max);
        fbuffer[i] = sample;
      }
      ttl_time[0] += t;
      ttl_time[1] += t;
      if (ttl_time[1] >= note_duration) {
        ttl_time[1] = 0.0;
      }
    }

    for (int f = 0; f < BL; f++) {
      double sample = fbuffer[f];
      if (sample < 0.0 || sample > 0.0) {
        fbuffer[f] = normalize_sample(sample, max);
        fbuffer[f] *= INT16_MAX * 0.5;
        sbuffer[f] = static_cast<int16_t>(fbuffer[f]);
      } else {
        sbuffer[f] = 0;
      }
    }
    buffer_enabled = 1;
  }
}

double Synth::generate_sample() {
  double sample = 0.0;
  for (auto& pair : *freq_map) {
    Freq_Data* fptr = &pair.second;
    if (!fptr->is_dead) {
      double wave     = create_layered_wave(fptr->freq, ttl_time[0]);
      double envelope = handle_envelope_gen(fptr->freq);
      sample += (wave * envelope);
    }
  }
  if (ttl_time[1] < note_duration) {
    for (auto& pair : *c_freq_map) {
      Freq_Data* fptr = &pair.second;
      if (!fptr->is_dead) {
        double wave     = create_layered_wave(fptr->freq, ttl_time[1]);
        double envelope = handle_envelope_gen(fptr->freq);
        sample += (wave * envelope) * 0.75;
      }
    }
  }
  return sample;
}

double Synth::create_chord_wave(double freq, double time) {
  double saw_base = saw_wave(freq, time);
  return bit_crusher(saw_base, 4);
}

double Synth::create_layered_wave(double freq, double time) {
  double saw_shift_pos = saw_wave(freq * (1.0 + 0.005), time);
  double saw_base      = saw_wave(freq, time);
  return ((bit_crusher(saw_base, 4) + bit_crusher(saw_shift_pos, 4))) / 2;
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

double Synth::normalize_sample(double sample, double absmax) { return sample / absmax; }

double Synth::w(double freq) { return 2.0 * freq * M_PI; }

double Synth::saw_wave(double freq, double time) {
  return (2.0 / M_PI) * (freq * M_PI * fmod(time, 1.0 / freq) - (M_PI / 2.0));
}
