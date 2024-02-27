#include "inc/main.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstring>
#include <math.h>

void generate_radians(Sint16 prebuffer[], int bufferlen) {}

void audio_callback(void *userdata, Uint8 *stream, int length) {
  Synth *SC = (Synth *)userdata;
  Sint16 *stream_ptr = (Sint16 *)stream;
  Sint16 prebuffer[length];
}
