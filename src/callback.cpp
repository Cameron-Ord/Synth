#include "inc/main.hpp"

void audio_cb(void* userdata, Uint8* stream, int bytes) {
  Synth* s = (Synth*)userdata;
  printf("BYTES : %d\n", bytes);
  if (s->get_enabled_state() == 1) {
    int16_t* b = s->get_sbuffer();
    memcpy(stream, b, bytes);
    s->set_enabled_state(0);
  }
}
