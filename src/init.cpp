#include "inc/main.hpp"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_timer.h>

Initializer::Initializer(std::set<int> *err) {
  init_sdl(err);
  create_window(err);
  create_renderer(err);
}
Initializer::~Initializer() {
  SDL_PauseAudio(1);
  SDL_PauseAudioDevice(dev, 1);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(w);
  SDL_CloseAudio();
  SDL_CloseAudioDevice(dev);
}

void Initializer::open_audio() {
  SDL_PauseAudioDevice(dev, 0);
  SDL_PauseAudio(0);
}

void Initializer::init_sdl(std::set<int> *err) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_Log("Unable to init SDL2 %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
}

void Initializer::create_window(std::set<int> *err) {
  this->w = SDL_CreateWindow("Synth", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, BWIDTH, BHEIGHT, 0);
  if (!this->w) {
    SDL_Log("Failed to create window! %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
  SDL_SetWindowResizable(this->w, SDL_TRUE);
}

void Initializer::create_renderer(std::set<int> *err) {
  this->r = SDL_CreateRenderer(
      this->w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!this->r) {
    SDL_Log("Failed to create renderer! %s", SDL_GetError());
    SDL_DestroyWindow(this->w);
    err->insert(-1);
  }
  err->insert(0);
}

void Initializer::create_spec(Synth *syn) {
  spec.freq = SR;
  spec.format = AUDIO_S16;
  spec.channels = 1;
  spec.samples = BL;
  spec.callback = audio_callback;
  spec.userdata = syn;
}

void Initializer::create_dev(std::set<int> *err) {
  dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
  if (!dev) {
    SDL_Log("Failed to open audio device %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
}

SDL_AudioDeviceID Initializer::get_device() { return dev; }
SDL_AudioSpec *Initializer::get_spec() { return &spec; }
SDL_Window *Initializer::get_window() { return w; }
SDL_Renderer *Initializer::get_renderer() { return r; }
