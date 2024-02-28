#include "inc/main.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
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
      note_samples[note] = 0.0;
      if (SC->playing[note]) {
        double freq = SC->frequency[note];
        double sine = sin(2 * M_PI * freq * time);
        note_samples[note] = sine;
        notes_playing += 1;
      } else {
        note_samples[note] = 0.0;
      }
      sample += note_samples[note];
    }
    if (notes_playing > 0) {
      double scaled_sample = sample * INT16_MAX;
      Sint16 casted_sample = static_cast<Sint16>(scaled_sample);
      stream_ptr[i] = abs(casted_sample);

      if (stream_ptr[i] > INT16_MAX) {
        stream_ptr[i] = INT16_MAX;
      } else if (stream_ptr[i] < INT16_MIN) {
        stream_ptr[i] = INT16_MIN;
      }

    } else {
      stream_ptr[i] = 0;
    }

    time += t;
  }
}
