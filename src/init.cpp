

#include "inc/main.hpp"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>

SDL_State::SDL_State(std::set<int>* err) {
  init_sdl(err);
  create_window(err);
  create_renderer(err);
}

SDL_State::~SDL_State() {
  SDL_PauseAudioDevice(dev, 1);
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(w);
  SDL_CloseAudioDevice(dev);
}

void SDL_State::open_audio() { SDL_PauseAudioDevice(dev, 0); }

void SDL_State::init_sdl(std::set<int>* err) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_Log("UNABLE TO INITIALIZE SDL2 %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
}

void SDL_State::create_window(std::set<int>* err) {
  w = SDL_CreateWindow("Synth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, BWIDTH, BHEIGHT, 0);
  if (!w) {
    SDL_Log("FAILED TO CREATE WINDOW! %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
  SDL_SetWindowResizable(w, SDL_TRUE);
}

void SDL_State::create_renderer(std::set<int>* err) {
  r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!r) {
    SDL_Log("FAILED TO CREATE RENDERER! %s", SDL_GetError());
    SDL_DestroyWindow(w);
    err->insert(-1);
  }
  err->insert(0);
}

void SDL_State::create_spec(Synth* syn) {
  spec.freq     = SR;
  spec.format   = AUDIO_S16;
  spec.channels = 2;
  spec.samples  = BL;
  spec.callback = audio_cb;
  spec.userdata = syn;
}

void SDL_State::create_dev(std::set<int>* err) {
  dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
  if (!dev) {
    SDL_Log("FAILED TO OPEN AUDIO DEVICE! %s", SDL_GetError());
    err->insert(-1);
  }
  err->insert(0);
}

SDL_AudioDeviceID SDL_State::get_device() { return dev; }
SDL_AudioSpec*    SDL_State::get_spec() { return &spec; }
SDL_Window*       SDL_State::get_window() { return w; }
SDL_Renderer*     SDL_State::get_renderer() { return r; }
