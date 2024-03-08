#include "inc/main.hpp"

void audio_callback(void *userdata, Uint8 *stream, int length) {
  Synth *s = (Synth *)userdata;
  if (s->buffer_flag == 1) {
    memcpy(stream, s->BUFFER_DATA, sizeof(Sint16) * (BUFFERSIZE / 2));
  }
  s->buffer_flag = 0;
}
