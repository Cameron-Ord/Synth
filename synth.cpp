#include "inc/main.hpp"
#include <math.h>
double SynthWrapper::w(double freq) { return 2.0 * freq * M_PI; }

void SynthWrapper::create_synth_ptrs() {
  this->ptr_arr[0] = &SynthWrapper::sawtooth;
  this->ptr_arr[1] = &SynthWrapper::square;
  this->ptr_arr[2] = &SynthWrapper::pulse_wave;
}

double SynthWrapper::sawtooth(double freq, double dtime) {
  time_scalar = 1.0;
  return (2.0 / M_PI) * (freq * M_PI * fmod((dtime * time_scalar), 1.0 / freq) - (M_PI / 2.0));
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

double SynthWrapper::pulse_wave(double freq, double dtime){
	double period = 1.0 / freq;
	double t = fmod(dtime, period);
	if(t < period / 2){
		return 1.0;
	} else {
		return -1.0;
	}
}

double SynthWrapper::modulator(double freq, double mi, double dtime){
	double modulation = sin(2*M_PI*1.0*dtime);
	double mfreq = freq + modulation * mi; 
	wave_fn_ptr ptr = this->ptr_arr[this->wave_ptr_index];
	double wave = (this->*ptr)(mfreq, dtime);
	return wave;
}

double SynthWrapper::call_func(double freq, double dtime){
	wave_fn_ptr ptr = this->ptr_arr[this->wave_ptr_index];
	double wave = (this->*ptr)(freq, dtime);
	return wave;
}
