#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <stk/ADSR.h>
#include <stk/Filter.h>
#include <stk/Fir.h>
#include <stk/FreeVerb.h>
#include <stk/Stk.h>
#include <vector>

#define SR 44100
#define BL 1024
#define NLEN 12
#define BHEIGHT 600
#define BWIDTH 800

void audio_callback(void *userdata, Uint8 *stream, int bytes);

class Initializer;
class Synth;
class Inputs;

class Initializer {
public:
  Initializer(std::set<int> *err);
  ~Initializer();
  void init_sdl(std::set<int> *err);
  void create_window(std::set<int> *err);
  void create_renderer(std::set<int> *err);
  void create_spec();
  void create_dev(std::set<int> *err);
  SDL_Window *get_window();
  SDL_Renderer *get_renderer();
  SDL_AudioSpec *get_spec();
  SDL_AudioDeviceID get_device();

private:
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioDeviceID dev;
  SDL_AudioSpec spec;
};

class Synth {
public:
  Synth(Initializer *init);
  ~Synth();
  void set_frequencies();
  void set_buffers();
  void set_params();
  void run_synth(Initializer *init);
  void add_spec_values(SDL_AudioSpec *spec);
  void handle_synth_key();
  std::pair<double *, int16_t *> get_buffers();
  std::set<std::pair<std::string, double>> get_params();
  std::vector<int> base_km;
  std::vector<double> base_freqs;
  size_t notes_len;

private:
  // freq , playing ---- dtime , amp
  std::map<std::pair<double, int>, std::pair<double, double>> frequencies;
  std::pair<double *, int16_t *> buffers;
  std::set<std::pair<std::string, double>> playback_params;
  std::set<int> held_keys;
  std::map<std::set<std::pair<double, int>>, std::function<void()>> KM;
};
class Renderer {};
