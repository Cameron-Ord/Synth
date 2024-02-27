#include "inc/main.hpp"
#include "inc/operations.hpp"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

Synth::Synth() {
  this->running = 0;
  this->startup_flag = 0;
  int err;
  err = this->init_sdl();
  if (err < 0) {
    startup_flag = -1;
  }
  err = this->init_audio();
  if (err < 0) {
    startup_flag = -1;
  }
  if (this->startup_flag == 0) {
    this->define_keymaps();
    this->setup_inputs();
    SDL_PauseAudioDevice(this->device, 0);
    SDL_PauseAudio(0);
    this->running = 1;
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
}

void Synth::setup_inputs() {
  double f[] = {130.81, 138.59, 146.83, 155.56, 164.81, 174.61,
                185.00, 196.00, 207.65, 220.00, 233.08, 246.94};
  memcpy(this->frequency, f, sizeof(double) * NOTES);
  memset(this->playing, 0, sizeof(int) * NOTES);
}

void Synth::define_keymaps() {
  int KEYMAPS[] = {Q, W, E, A, S, D, J, K, L, U, I, O};
  memcpy(this->KM, KEYMAPS, sizeof(int) * NOTES);
  quicksort(this->KM, 0, 11);
}

int Synth::init_sdl() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_Log("Unable to init SDL2 %s", SDL_GetError());
    return -1;
  }

  this->w = SDL_CreateWindow("Synth", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  if (!this->w) {
    SDL_Log("Failed to create window! %s", SDL_GetError());
    return -1;
  }

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
