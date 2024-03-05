#include "inc/main.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cmath>
#include <cstdint>
#include <math.h>

double low_pass(double input, double alpha, double last_sample) {
  return alpha * input + (1 - alpha) * last_sample;
}

void audio_callback(void *userdata, Uint8 *stream, int length) {}

// int STW = length / sizeof(Sint16);
// Synth *SC = (Synth *)userdata;
// Sint16 *streamptr = reinterpret_cast<Sint16 *>(stream);
// Sint16 intermediary_buffer[STW];
//
// static double time = 0.0;
// static double note_timer = 0.0;
// double note_samples[NOTES];
// double total_duration = 60.0 / SC->tempo;
// double note_duration = 60.0 / SC->tempo * SC->note_notation;
// double AMP_MAX = 1.0;
// double amp = 1.0;
// double last_sample = 0.0;
//
// for (int i = 0; i < BUFFERSIZE; i++) {
// double sample = 0.0;
// double sum = 0.0;
// int notes_playing = 0;
// int playing_flag = 1;
//
// if (time >= note_timer + note_duration) {
// playing_flag = 0;
//}
//
// if(playing_flag){
// for (int note = 0; note < NOTES; note++) {
// note_samples[note] = 0.0;
// if (SC->playing[note]) {
// double freq = SC->frequency[note];
// double NT = fmod(time - note_timer, note_duration);
// double envelope = 0.0;
// if (NT < SC->AT) {
// envelope = NT / SC->AT;
//} else if (NT < SC->AT + SC->DT) {
// envelope = 1.0 - (NT - SC->AT) / SC->DT * (1.0 - SC->SL);
//} else if (NT < note_duration - SC->RT) {
// envelope = SC->SL;
//} else {
// envelope = (note_duration - NT) / SC->RT;
//}
//
// double wave = SC->ptr_arr[SC->wave_ptr_index](freq, time);
// double package = wave;
// note_samples[note] = package * envelope;
// notes_playing += 1;
// sum += note_samples[note];
//}
//}
//}
//
// if (notes_playing > 0) {
//
// double nss = sum / notes_playing;
// if (nss > 1.0 || nss < -1.0) {
// amp = AMP_MAX / fabs(nss);
//}
//
// sample = nss * amp;
//
// if (sample > 1.0) {
// sample = 1.0;
//} else if (sample < -1.0) {
// sample = -1.0;
//}
//
// double alpha = 0.25;
// double fsample = low_pass(sample, alpha, last_sample);
// last_sample = fsample;
// double scaled_fsample = fsample * (INT16_MAX);
// intermediary_buffer[i] = static_cast<Sint16>(round(scaled_fsample));
//
//} else {
// intermediary_buffer[i] = 0;
//}
//
// time += SC->t;
// if (time >= note_timer + note_duration) {
// note_timer += note_duration;
//}
//
// if (time >= total_duration) {
// time = 0.0;
// note_timer = 0.0;
//}
//
// if (intermediary_buffer[i] > INT16_MAX) {
// intermediary_buffer[i] = INT16_MAX;
//} else if (intermediary_buffer[i] < INT16_MIN) {
// intermediary_buffer[i] = INT16_MIN;
//}
//}
// memcpy(streamptr, intermediary_buffer, sizeof(Sint16)*BUFFERSIZE);
// memcpy(SC->BUFFER_DATA, intermediary_buffer, sizeof(Sint16) * BUFFERSIZE);
