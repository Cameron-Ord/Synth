#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cstdint>
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
  void poll_events();
  void keydown(int KEYCODE);
  void keyup(int KEYCODE);
  void set_frequencies();
  void set_buffers();
  void set_params();
  void run_synth(Initializer *init);
  void add_spec_values(SDL_AudioSpec *spec);
  void handle_synth_key(double freq, int *playing);
  std::pair<double *, int16_t *> get_buffers();
  std::map<std::string, double> get_params();

private:
  int running;
  std::vector<int> base_km;
  std::vector<double> base_freqs;
  size_t notes_len;
  // vector that will be added to, or removed from based on inputs. Used for
  // iteratively generating samples
  std::vector<std::pair<double, int>> playing_freqs;
  // buffer pair
  std::pair<double *, int16_t *> buffers;
  // mapping params to their names
  std::map<std::string, double> params;
  // mapping the frequencies to scancodes
  std::map<int, std::pair<double, int>> frequencies;
  // set to track held keys.
  std::set<int> held_keys;
};
class Renderer {};
