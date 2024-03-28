#include "inc/main.hpp"
#include <cstdint>

void Synth::create_sample_buffer() {
  if (!buffer_enabled) {
    double left_max  = 0.0;
    double right_max = 0.0;
    for (int i = 0; i < BL; i++) {
      double left_sample  = create_left_sample();
      double right_sample = create_right_sample();
      update_left_max(&left_max, left_sample);
      update_right_max(&right_max, right_sample);
      left_buffer[i]  = left_sample;
      right_buffer[i] = right_sample;
      update_time();
    }

    for (int f = 0; f < BL; f++) {
      process_left_sample(&left_buffer[f], left_max);
      process_right_sample(&right_buffer[f], right_max);
      fill_audio_output(f);
      fill_with_zero(f);
    }
  }
  buffer_enabled = 1;
}

void Synth::update_right_max(double* max, double right_sample) { set_max_value(right_sample, max); }

void Synth::update_left_max(double* max, double left_sample) { set_max_value(left_sample, max); }

void Synth::fill_with_zero(int f) {
  if (left_buffer[f] == 0.0) {
    sbuffer[f * 2] = 0;
  } else if (right_buffer[f] == 0.0) {
    sbuffer[f * 2 + 1] = 0;
  }
}

void Synth::fill_audio_output(int f) {
  left_buffer[f] *= INT16_MAX * 0.5;
  right_buffer[f] *= INT16_MAX * 0.5;
  sbuffer[f * 2]     = static_cast<int16_t>(left_buffer[f]);
  sbuffer[f * 2 + 1] = static_cast<int16_t>(right_buffer[f]);
}

void Synth::process_left_sample(double* sample, double left_max) {
  if (*sample < 0.0 || *sample > 0.0) {
    normalize_sample(sample, left_max);
  }
}

void Synth::process_right_sample(double* sample, double right_max) {
  if (*sample < 0.0 || *sample > 0.0) {
    normalize_sample(sample, right_max);
  }
}

void Synth::update_time() {
  ttl_time[0] += t;
  ttl_time[1] += t;
}

double Synth::create_left_sample() {
  double total = 0.0;
  total += generate_chord_sample();
  total += generate_sample();
  return total;
}

double Synth::create_right_sample() {
  double total = 0.0;
  total += generate_chord_sample();
  total += generate_sample();
  return total;
}

double Synth::generate_chord_sample() {
  double sample = 0.0;
  for (auto& pair : *c_freq_map) {
    Freq_Data* fptr = &pair.second;
    if (!fptr->is_dead) {
      double wave     = create_layered_wave(fptr->freq, ttl_time[1]);
      double envelope = handle_envelope_gen(fptr->freq);
      sample += (wave * envelope) * 0.75;
    }
  }
  return sample;
}

double Synth::generate_sample() {
  double sample = 0.0;
  for (auto& pair : *freq_map) {
    Freq_Data* fptr = &pair.second;
    if (!fptr->is_dead) {
      double wave     = create_layered_wave(fptr->freq, ttl_time[0]);
      double envelope = handle_envelope_gen(fptr->freq);
      sample += DRC((bit_crusher(wave, 8) * envelope), 0.5, 2.5) * 0.8;
    }
  }
  return sample;
}

double Synth::create_chord_wave(double freq, double time) {
  double saw_base = saw_wave(freq, time);
  return bit_crusher(saw_base, 16);
}

double Synth::create_layered_wave(double freq, double time) {
  double saw_base    = saw_wave(freq, time);
  double square_base = square_wave(freq, time);
  double pulse_base  = pulse_wave(freq, time, 0.75);
  return saw_base;
}

double Synth::bit_crusher(double sample, int bit_depth) {
  double max_value = pow(2, bit_depth) - 1;
  return round(sample * max_value) / max_value;
}

double Synth::wave_shape(double sample) { return sin(sample * 2) * 0.5; }

double Synth::wave_fold(double sample, double amount) {
  return amount * (2 * fabs(fmod(sample / amount + 1, 2.0)) - 1);
}

double Synth::fuzzer(double sample, double amount) { return tanh(sample * amount); }

double Synth::hard_clip(double sample, double threshold) {
  if (sample > threshold) {
    return threshold;
  } else if (sample < -threshold) {
    return -threshold;
  } else {
    return sample;
  }
}

double Synth::phase_distort(double sample, double amount) {
  return sin(sample + amount * sin(sample));
}

double Synth::DRC(double sample, double threshold, double ratio) {
  if (sample > threshold) {
    return threshold + (sample - threshold) / ratio;
  } else {
    return sample;
  }
}

void Synth::set_max_value(double sample, double* max) {
  double max_of_sample = fmax(*max, fabs(sample));
  if (max_of_sample >= *max) {
    *max = max_of_sample;
  }
}

void Synth::normalize_sample(double* sample, double absmax) { *sample /= absmax; }

double Synth::w(double freq) { return 2.0 * freq * M_PI; }

double Synth::saw_wave(double freq, double time) {
  return (2.0 / M_PI) * (freq * M_PI * fmod(time, 1.0 / freq) - (M_PI / 2.0));
}

double Synth::square_wave(double freq, double time) {
  return (sin(2 * M_PI * freq * time) > 0) ? -1 : 1;
}

double Synth::pulse_wave(double freq, double time, double duty_cycle) {
  double period          = 1.0 / freq;
  double pulse_width     = duty_cycle * period;
  double normalized_time = fmod(time, period);
  if (normalized_time < pulse_width) {
    return 1.0;
  } else {
    return -1.0;
  }
}
