#include "inc/main.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <cstring>
#include <math.h>

void audio_callback(void *userdata, Uint8 *stream, int length) {
  Synth *SC = (Synth *)userdata;
  Sint16 *stream_ptr = (Sint16 *)stream;
  double t = 1.0 / SAMPLERATE;
  double time = 0.0;
  double note_samples[NOTES];

  for (int i = 0; i < BUFFERSIZE; i++) {
    double sample = 0.0;
    int notes_playing = 0;
    for (int note = 0; note < NOTES; note++) {
      if (SC->playing[note]) {
        double freq = SC->frequency[note];
        double sawtooth = fmod(2 * time * 293.11, 2.0) - 1.0;

        note_samples[note] = sawtooth;
        notes_playing += 1;
      } else {
        note_samples[note] = 0.0;
      }
      sample += note_samples[note];
    }
    if (notes_playing > 0) {
      double scaled_sample = sample * (INT16_MAX / notes_playing);
      Sint16 casted_sample = static_cast<Sint16>(scaled_sample);
      stream_ptr[i] = casted_sample;
    } else {
      stream_ptr[i] = 0;
    }

    time += t;
  }
}
