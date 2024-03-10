#include "inc/main.hpp"

void audio_callback(void *userdata, Uint8 *stream, int bytes) {
  Synth *s = (Synth *)userdata;
  if (s->buffer_enabled) {
    std::pair<double *, int16_t *> b = s->get_buffers();
    memcpy(stream, b.second, sizeof(int16_t) * BL);
    s->buffer_enabled = 0;
  }
}
