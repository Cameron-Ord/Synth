#include "inc/main.hpp"

void Synth::generate_samples(SynthWrapper *synfunc) {
  if (!this->buffer_flag) {
    for (int i = 0; i < BUFFERSIZE; i++) {
      double sample = 0.0;
      double ssum = 0.0;
      double last_sample = 0.0;
      this->create_sample_radians(synfunc, &sample, &ssum, &last_sample);
      double scaled_sample = sample * INT16_MAX;
      this->BUFFER_DATA[i] = static_cast<Sint16>(round(scaled_sample * 0.75));
    }
    this->buffer_flag = 1;
  }
}

void Synth::create_sample_radians(SynthWrapper *synfunc, double *sam,
                                  double *ssum, double *ls) {
  int notes_playing = 0;
  double AMP_MAX = 1.0;
  double amp = 1.0;
  double note_samples[NOTES];
  double t = 1.0 / SAMPLERATE;
  for (int note = 0; note < NOTES; note++) {
    note_samples[note] = 0.0;
    if (this->playing[note]) {
      double freq = this->frequency[note];
      typedef double (SynthWrapper::*wave_fn_ptr)(double, double);
      wave_fn_ptr ptr = synfunc->ptr_arr[synfunc->wave_ptr_index];
      double wave = (synfunc->*ptr)(freq, this->times[note]);
      this->times[note] += t;
      double package = wave;
      note_samples[note] = package;
      notes_playing += 1;
      *ssum += note_samples[note];
    } else if (!this->playing[note]) {
      this->times[note] = 0.0;
      *ssum += note_samples[note];
    }
    double nss = this->normalize_radians(*ssum, notes_playing, &amp, AMP_MAX);
    *sam = nss * amp;
    if (*sam > 1.0) {
      *sam = 1.0;
    } else if (*sam < -1.0) {
      *sam = -1.0;
    }
  }
}

double Synth::normalize_radians(double ssum, int notes_playing, double *amp,
                                double AMP_MAX) {
  double nss = ssum / notes_playing;
  if (nss > 1.0 || nss < -1.0) {
    *amp = AMP_MAX / fabs(nss);
  }
  return nss;
}
// double Synth::generate_envelope(SynthWrapper *synfunc){

//}
