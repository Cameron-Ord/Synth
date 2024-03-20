#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define SR 44100
#define BL 2048
#define NOTELEN 12
#define BHEIGHT 800
#define BWIDTH 1200

void audio_cb(void* userdata, Uint8* stream, int bytes);
class SDL_State;
class Synth;
class Inputs;
class Renderer;
struct Freq_Data {
  int    is_dead;
  double freq;
  double time;
};

struct ADSR_PARAMS {
  double AT;
  double DT;
  double SL;
  double RT;
  double envelope;
};

void cleanup(SDL_State* st, Inputs* in, Synth* syn, Renderer* ren);
class SDL_State {
public:
  SDL_State(std::set<int>* err);
  ~SDL_State();
  void              init_sdl(std::set<int>* err);
  void              create_window(std::set<int>* err);
  void              create_renderer(std::set<int>* err);
  void              create_spec(Synth* syn);
  void              create_dev(std::set<int>* err);
  void              open_audio();
  SDL_Window*       get_window();
  SDL_Renderer*     get_renderer();
  SDL_AudioSpec*    get_spec();
  SDL_AudioDeviceID get_device();

private:
  SDL_Window*       w;
  SDL_Renderer*     r;
  SDL_AudioSpec     spec;
  SDL_AudioDeviceID dev;
};

class Synth {
public:
  Synth();
  ~Synth();
  double                    bit_crusher(double sample, int bit_depth);
  double                    handle_envelope_gen(double time);
  double                    set_attack_env(double time);
  double                    set_decay_env(double time);
  double                    generate_chord_sample();
  double                    set_sustain_env();
  double                    create_layered_wave(double freq, double time);
  double                    create_chord_wave(double freq, double time);
  double                    get_max_value(double sample, double max);
  double                    normalize_sample(double sample, double absmax);
  double                    w(double freq);
  double                    saw_wave(double freq, double time);
  double                    generate_sample();
  void                      set_adsr();
  void                      create_sample_buffer();
  void                      set_defaults(std::vector<int>* base_km, std::vector<int>* alt_km);
  void                      set_buffers();
  void                      set_run_state(int r);
  int                       get_enabled_state();
  void                      set_enabled_state(int en);
  int*                      get_run_state();
  double*                   get_fbuffer();
  int16_t*                  get_sbuffer();
  std::map<int, Freq_Data>* get_freq_map();
  std::map<int, Freq_Data>* get_cfreq_map() { return c_freq_map; }
  int                       get_buffer_status();

private:
  double                    note_duration;
  double                    ttl_time[2];
  double                    t;
  double*                   fbuffer;
  int16_t*                  sbuffer;
  int                       running;
  int                       buffer_enabled;
  double*                   samples;
  std::vector<double>       base_freqs;
  std::map<int, Freq_Data>* freq_map;
  std::map<int, Freq_Data>* c_freq_map;
  ADSR_PARAMS*              adsr;
  int                       notes_len;
};

class Renderer {
public:
  Renderer();
  void do_render(SDL_State* init, Synth* syn);
  void apply_win_resize(SDL_State* init);
  void update_render_values();

private:
  float cell_width;
  float heightf;
  int   win_width;
  int   win_height;
};

class Inputs {
public:
  void              poll_events(Synth* syn, Renderer* rend, SDL_State* init);
  void              clear_chord_pipe(Synth* syn);
  void              keydown(int KEYCODE, Synth* syn);
  void              keyup(int SCANCODE, Synth* syn);
  void              synth_key_on(Freq_Data* fd);
  void              synth_key_off(Freq_Data* fd);
  void              chord_keydown(int KEYCODE, Synth* syn);
  void              chord_key_on(Freq_Data* cfd);
  void              chord_key_off(int KEYCODE, Synth* syn);
  std::set<int>*    get_held_keys_ptr() { return &held_keys; }
  std::set<int>     get_held_keys() { return held_keys; }
  std::vector<int>* get_base_km_ptr() { return &base_km; };
  std::vector<int>  get_base_km() { return base_km; };
  std::vector<int>* get_alt_km_ptr() { return &alt_km; }

private:
  std::set<int>    held_chord_keys;
  std::set<int>    held_keys;
  std::vector<int> base_km;
  std::vector<int> alt_km;
};
