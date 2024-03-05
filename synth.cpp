#include "inc/main.hpp"
#include <math.h>

void SynthWrapper::create_synth_ptrs() {
  this->ptr_arr[0] = &SynthWrapper::sawtooth;
  this->ptr_arr[1] = &SynthWrapper::sine;
  this->ptr_arr[2] = &SynthWrapper::triangle;
  this->ptr_arr[3] = &SynthWrapper::square;
  this->ptr_arr[4] = &SynthWrapper::tanh_wave;
  this->ptr_arr[5] = &SynthWrapper::cosine;
}

double SynthWrapper::sawtooth(double freq, double time) {
  double phase = fmod(time * freq, 1.0);
  double sawtooth = 2.0 * phase - 1.0;
  return sawtooth;
}

double SynthWrapper::tanh_wave(double freq, double time) {
  double period = 1.0 / freq;
  double time_scaled = time / period;
  return 1.0 * tanh(2 * sin(2 * M_PI * freq * time_scaled));
}

double SynthWrapper::sine(double freq, double time) {
  return sin(2 * M_PI * freq * time);
}

double SynthWrapper::cosine(double freq, double time) {
  return cos(2 * M_PI * freq * time);
}

double SynthWrapper::square(double freq, double time) {
  double square = fmod(sin(2 * M_PI * freq * time), 1.0) > 0 ? 1.0 : -1.0;
  return square;
}

double SynthWrapper::triangle(double freq, double time) {
  double period = 1.0 / freq;
  double half_period = period / 2.0;
  double slope = 4.0 * 1.0 / period;
  double time_mod = fmod(time, period);
  if (time_mod < half_period)
    return slope * time_mod - 1.0;
  else
    return -slope * (time_mod - half_period) + 1.0;
}

double SynthWrapper::distort(double in, double amount) {
  return tanh(in * amount);
}
