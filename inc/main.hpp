#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
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
#include <utility>
#include <vector>

#define SR 44100
#define BL 2048
#define NLEN 12
#define BHEIGHT 600
#define BWIDTH 800

void audio_callback(void *userdata, Uint8 *stream, int bytes);

class Initializer;
class Synth;
class Inputs;

class Initializer {
public:
  Initializer(std::set<int> *err, Synth *syn);
  ~Initializer();

  void run_synth(Synth *syn);
  void open_audio();
  void init_sdl(std::set<int> *err);
  void create_window(std::set<int> *err);
  void create_renderer(std::set<int> *err);
  void create_spec(Synth *syn);
  void create_dev(std::set<int> *err);
  SDL_Window *get_window();
  SDL_Renderer *get_renderer();
  SDL_AudioSpec *get_spec();
  SDL_AudioDeviceID get_device();

private:
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioSpec spec;
  SDL_AudioDeviceID dev;
};

class Synth {
public:
  Synth();
  ~Synth();
  void set_fir_filter();
  void set_adsr();
  void set_freeverb();
  double get_max_value(double sample, double max);
  double normalize_sample(double sample, double absmax);
  void create_sample_buffer();
  void poll_events();
  void keydown(int KEYCODE);
  void keyup(int KEYCODE);
  void set_frequencies();
  void set_buffers();
  void set_params();
  double w(double freq);
  double generate_wave(double freq, double time);
  void synth_key_on(double freq, double *time);
  void synth_key_off(double freq, double *time);
  double generate_sample();
  int get_enabled_state();
  int *get_run_state();
  double *kaiser_window(double beta, int filter_length);
  double *generate_coefficients(int filter_length, double beta);
  std::pair<double *, int16_t *> get_buffers();
  std::map<std::string, double> get_params();
  int buffer_enabled;

private:
  double *coeffs;
  stk::Fir filter;
  int filt_len;
  stk::ADSR adsr;
  stk::FreeVerb verb;
  double envelope;
  int running;
  std::vector<int> base_km;
  std::vector<double> base_freqs;
  size_t notes_len;
  /*
   vector that will be added to, or removed from based on inputs. Used for
   iteratively generating samples
   usefull in that I dont have to interate 12 times to check if a note is
   playing, only for the size of the vector
  */
  std::vector<std::pair<double, double *>> *playing_freqs;

  // buffer pair
  std::pair<double *, int16_t *> buffers;
  // mapping params to their names
  std::map<std::string, double> params;
  // mapping the frequencies to scancodes
  std::map<int, std::pair<double, double>> *frequencies;
  // set to track held keys.
  std::set<int> held_keys;
};

class Renderer {
public:
  void do_render(Initializer *init, Synth *syn);

private:
};
