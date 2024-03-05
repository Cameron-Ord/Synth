#include "inc/main.hpp"
#include "inc/operations.hpp"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <map>
#include <utility>

Synth::Synth() {
  this->define_keymaps();
  this->setup_inputs();
  this->set_default_buffer();
  this->create_default_settings();
  this->create_synth_ptrs();
  this->running = 0;
  this->startup_flag = 0;
  this->render_flag = 0;
  this->wave_ptr_index = 0;
  int err;

  this->w = NULL;
  err = this->create_window();
  if (err < 0) {
    startup_flag = -1;
  }

  this->r = NULL;
  err = this->create_renderer();
  if (err < 0) {
    startup_flag = -1;
  }

  err = this->init_audio();
  if (err < 0) {
    startup_flag = -1;
  }

  if (this->startup_flag == 0) {
    SDL_PauseAudioDevice(this->device, 0);
    SDL_PauseAudio(0);
    this->running = 1;
    this->render_flag = 1;
  }
}

Synth::~Synth() {
  if (this->r) {
    SDL_DestroyRenderer(this->r);
  }
  if (this->w) {
    SDL_DestroyWindow(this->w);
  }
  if (this->device) {
    SDL_CloseAudioDevice(this->device);
  }
  SDL_CloseAudio();
  delete[] this->frequency;
  delete[] this->playing;
  delete this->KM;
}

void Synth::create_default_settings() {
  this->AT = 0.8;
  this->DT = 0.3;
  this->SL = 0.8;
  this->RT = 0.4;

  this->t = 1.0 / SAMPLERATE;
  this->note_notation = 0.5;
  this->tempo = 90;
}

void Synth::create_synth_ptrs() {
  this->ptr_arr[0] = &sawtooth;
  this->ptr_arr[1] = &sine;
  this->ptr_arr[2] = &triangle;
  this->ptr_arr[3] = &square;
  this->ptr_arr[4] = &tanh_wave;
  this->ptr_arr[5] = &cosine;
}

void Synth::set_default_buffer() {
  memset(this->BUFFER_DATA, 0, sizeof(Sint16) * BUFFERSIZE);
}

void Synth::setup_inputs() {
  double f[] = {130.81,     138.59,     146.83,     155.56,     164.81,
                174.61,     185.00,     196.00,     207.65,     220.00,
                233.08,     246.94,     130.81 * 2, 138.59 * 2, 146.83 * 2,
                155.56 * 2, 164.81 * 2, 174.61 * 2, 185.00 * 2, 196.00 * 2,
                207.65 * 2, 220.00 * 2, 233.08 * 2, 246.94 * 2};

  this->frequency = new double[NOTES];
  this->playing = new int[NOTES];

  for (int k = 0; k < NOTES; k++) {
    this->frequency[k] = f[k];
  }

  for (int i = 0; i < NOTES; i++) {
    this->playing[i] = 0;
  }
}

void Synth::define_keymaps() {
  int SYNTH_KEYMAPS[] = {Q, W, E, A, S, D, J, K, L, U, I, O,
                         Z, X, C, B, N, M, F, G, H, R, T, Y};

  int CONTROLS_KEYMAPS[] = {LARROW, RARROW};
  this->KM = new std::map<int, std::pair<int, int>>;
  for (int i = 0; i < NOTES; i++) {
    std::pair<int, int> index_and_call = std::make_pair(i, 1);
    this->KM->emplace(std::make_pair(SYNTH_KEYMAPS[i], index_and_call));
  }

  int LEN = sizeof(CONTROLS_KEYMAPS) / sizeof(CONTROLS_KEYMAPS[0]);
  for (int c = 0; c < LEN; c++) {
    std::pair<int, int> index_and_call = std::make_pair(c, 2);
    this->KM->emplace(std::make_pair(CONTROLS_KEYMAPS[c], index_and_call));
  }
}

int Synth::create_window() {
  this->w = SDL_CreateWindow("Synth", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, BWIDTH, BHEIGHT, 0);
  if (!this->w) {
    SDL_Log("Failed to create window! %s", SDL_GetError());
    return -1;
  }

  SDL_SetWindowResizable(this->w, SDL_FALSE);
  return 0;
}

int Synth::create_renderer() {
  this->r = SDL_CreateRenderer(
      this->w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!this->r) {
    SDL_Log("Failed to create renderer! %s", SDL_GetError());
    SDL_DestroyWindow(this->w);
    return -1;
  }
  return 0;
}

int Synth::init_audio() {
  this->spec.freq = SAMPLERATE;
  this->spec.format = AUDIO_S16;
  this->spec.channels = 1;
  this->spec.samples = BUFFERSIZE;
  this->spec.callback = audio_callback;
  this->spec.userdata = this;
  this->device = SDL_OpenAudioDevice(NULL, 0, &this->spec, NULL, 0);
  if (!this->device) {
    SDL_Log("Failed to open audio device %s", SDL_GetError());
    return -1;
  }
  return 0;
}
