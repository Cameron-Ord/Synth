#include "inc/main.hpp"


void Synth::generate_samples(SynthWrapper *synfunc) {
  if (!this->buffer_flag) {
    for (int i = 0; i < BUFFERSIZE; i++) {
      double sample = 0.0;
      double ssum = 0.0;
      double last_sample = 0.0;
      this->create_sample_radians(synfunc, &sample, &ssum, &last_sample);
      double scaled_sample = sample * INT16_MAX;
      this->BUFFER_DATA[i] = static_cast<Sint16>(scaled_sample);
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
  double t = 60.0 / (SAMPLERATE * this->tempo);

	ADSRGen adsr;
	for (int note = 0; note < NOTES; note++) {
		 if (this->playing[note]) {
		 double freq = this->frequency[note];
		 wave_fn_ptr ptr = synfunc->ptr_arr[synfunc->wave_ptr_index];
		 double wave = (synfunc->*ptr)(freq, this->times[note]);
		 this->times[note] += t;
		 double package = wave;
		 adsr.handle_envelope_generation(note, this->times[note]);
		 note_samples[note] = synfunc->distort(package, 0.8) * adsr.envelope[note];
		 notes_playing += 1;
		 *ssum += note_samples[note];
	 } else if (!this->playing[note] && this->times[note] > 0.0) {
		 double package = note_samples[note];
		 adsr.set_release_env(note);
		 note_samples[note] = package * adsr.envelope[note]; 
		 *ssum += note_samples[note];
		 if(note_samples[note] <= 0.0000005){
			this->times[note] = 0.0;
		 }
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
