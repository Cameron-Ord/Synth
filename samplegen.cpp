#include "inc/main.hpp"
#include <cstdint>

ADSRGen adsr;

void Synth::generate_samples(SynthWrapper *synfunc) {
  double absmax = 0.0;
  double samples[BUFFERSIZE];
  if (!this->buffer_flag) {
    for (int i = 0; i < BUFFERSIZE; i++) {
      double sample = 0.0;
      double ssum = 0.0;
      this->create_sample_radians(synfunc, &sample, &ssum);
      if (sample < 0.0 || sample > 0.0) {
        absmax = this->get_max_value(sample, absmax);
      }
      samples[i] = sample;
    }

    for (int d = 0; d < BUFFERSIZE; d++) {
      if (samples[d] < 0.0 || samples[d] > 0.0) {
        double normalized_sample = this->normalize_sample(samples[d], absmax);
        samples[d] = normalized_sample;
      }

      samples[d] *= INT16_MAX * 0.6;

      this->BUFFER_DATA[d] = static_cast<Sint16>(samples[d]);
    }
    this->buffer_flag = 1;
  }
}

// double scaled_sample = sample * INT16_MAX;
// this->BUFFER_DATA[i] = static_cast<Sint16>(scaled_sample);
void Synth::create_sample_radians(SynthWrapper *synfunc, double *sam,
                                  double *ssum) {
  double note_samples[NOTES];
  double t = 60.0 / (SAMPLERATE * this->tempo);

  for (int note = 0; note < NOTES; note++) {
     if (this->playing[note]) {
      double package = this->generate_wave(synfunc, note, t);
      note_samples[note] = this->create_envelope(synfunc, note, package);
      *ssum += note_samples[note]; 
    } else if (!this->playing[note] && this->times[note] > 0.0) {
      note_samples[note] *= this->handle_release(note);
      *ssum += note_samples[note] ;
      if (note_samples[note] <= 0.0000005) {
        this->times[note] = 0.0;
      }
    } else {
      this->times[note] = 0.0;
      note_samples[note] = 0.0;
    }
  }
  *sam = *ssum;
}

double Synth::handle_release(int n){
      adsr.set_release_env(n);
      return adsr.envelope[n];
}

double Synth::create_envelope(SynthWrapper *synfunc, int n, double package){
      adsr.handle_envelope_generation(n, this->times[n]);
      double sample = synfunc->distort(package, 0.8) * adsr.envelope[n];
      return sample;
}

double Synth::generate_wave(SynthWrapper *synfunc, int n, double t){
	double freq = this->frequency[n];
	wave_fn_ptr ptr = synfunc->ptr_arr[synfunc->wave_ptr_index];
      	double wave = (synfunc->*ptr)(freq, this->times[n]);
 	this->times[n] += t;
	return wave;
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
