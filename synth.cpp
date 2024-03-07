#include "inc/main.hpp"
#include <math.h>
double SynthWrapper::w(double freq) { return 2.0 * freq * M_PI; }

void SynthWrapper::create_synth_ptrs() {
  this->ptr_arr[0] = &SynthWrapper::sawtooth;
  this->ptr_arr[1] = &SynthWrapper::square;
}

double SynthWrapper::sawtooth(double freq, double dtime) {
  return (2.0 / M_PI) * (freq * M_PI * fmod(dtime, 1.0 / freq) - (M_PI / 2.0));
}

double SynthWrapper::sine(double freq, double dtime) {
  return sin(w(freq) * dtime);
}

double SynthWrapper::square(double freq, double dtime) {
  return sin(w(freq) * dtime) > 0.0 ? 1.0 : -1.0;
}

double SynthWrapper::triangle(double freq, double dtime) {
  return asin(sin(w(freq) * dtime)) * 2.0 / M_PI;
}

double SynthWrapper::distort(double in, double amount) {
  return tanh(in * amount);
}
