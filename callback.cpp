#include "inc/main.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cmath>
#include <cstdint>
#include <math.h>

const double AT = 0.1;
const double DT = 0.1;
const double SL = 0.5;
const double RT = 0.1;

double distort(double in, double amount) { return tanh(in * amount); }

void audio_callback(void *userdata, Uint8 *stream, int length) {
  Synth *SC = (Synth *)userdata;
  Sint16 *stream_ptr = reinterpret_cast<Sint16 *>(stream);
  Sint16 intermediary_buffer[BUFFERSIZE];
  double t = 1.0 / SAMPLERATE;
  static double time = 0.0;
  static double note_timer = 0.0;
  double note_samples[NOTES];
  int tempo = 120;
  double total_duration = 60.0 / tempo;
  double note_duration = 60.0 / tempo * 0.5;
  double AMP_MAX = 1.0;
  double amp = 1.0;

  for (int i = 0; i < BUFFERSIZE; i++) {
    double sample = 0.0;
    double sum = 0.0;
    int notes_playing = 0;
    int playing_flag = 1;

    if (time >= note_timer + note_duration) {
      playing_flag = 0;
    }

    for (int note = 0; note < NOTES; note++) {
      note_samples[note] = 0.0;
      if (SC->playing[note] && playing_flag) {
        double freq = SC->frequency[note];
        double phase = fmod(time * freq, 1.0);
        double sawtooth = 2.0 * phase - 1.0;
        // double sine = sin(2.0 * M_PI * freq * time);
        //  double square = fmod(sin(2 * M_PI * freq * time), 1.0) > 0 ? 1.0 :
        //  -1.0; double envelope = 1.0 - fabs(time - note_timer - note_duration
        //  / 2) / (note_duration / 2);
        double NT = fmod(time - note_timer, note_duration);
        double envelope = 0.0;
        if (NT < AT) {
          envelope = NT / AT;
        } else if (NT < AT + DT) {
          envelope = 1.0 - (NT - AT) / DT * (1.0 - SL);
        } else if (NT < note_duration - RT) {
          envelope = SL;
        } else {
          envelope = (note_duration - NT) / RT;
        }

        double package = distort(sawtooth, 0.75);
        note_samples[note] = package * envelope;
        notes_playing += 1;
        sum += note_samples[note];
      }
    }

    if (notes_playing > 0) {

      double nss = sum / notes_playing;
      if (nss > 1.0 || nss < -1.0) {
        amp = AMP_MAX / fabs(nss);
      }

      sample = nss * amp;

      if (sample > 1.0) {
        sample = 1.0;
      } else if (sample < -1.0) {
        sample = -1.0;
      }

      double scaled_sample = sample * INT16_MAX;
      Sint16 casted_sample = static_cast<Sint16>(scaled_sample);
      intermediary_buffer[i] = casted_sample;
      if (intermediary_buffer[i] > INT16_MAX) {
        intermediary_buffer[i] = INT16_MAX;
      } else if (intermediary_buffer[i] < INT16_MIN) {
        intermediary_buffer[i] = INT16_MIN;
      }
      stream_ptr[i] = intermediary_buffer[i];
    } else {
      stream_ptr[i] = 0;
    }

    time += t;
    if (time >= note_timer + note_duration) {
      note_timer += note_duration;
    }

    if (time >= total_duration) {
      time = 0.0;
      note_timer = 0.0;
    }
  }
  memcpy(SC->BUFFER_DATA, stream_ptr, sizeof(Sint16) * BUFFERSIZE);
}
