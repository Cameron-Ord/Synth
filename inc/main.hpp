#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <map>
#include <set>

#define BHEIGHT 800
#define BWIDTH 1600
#define NOTES 24
#define SAMPLERATE 44100
#define BUFFERSIZE 1024
#define ARR_LEN(x) sizeof(x) / sizeof(x[0])

typedef enum {
  Q = SDL_SCANCODE_Q,
  W = SDL_SCANCODE_W,
  E = SDL_SCANCODE_E,
  A = SDL_SCANCODE_A,
  S = SDL_SCANCODE_S,
  D = SDL_SCANCODE_D,
  J = SDL_SCANCODE_J,
  K = SDL_SCANCODE_K,
  L = SDL_SCANCODE_L,
  U = SDL_SCANCODE_U,
  I = SDL_SCANCODE_I,
  O = SDL_SCANCODE_O,
  Z = SDL_SCANCODE_Z,
  X = SDL_SCANCODE_X,
  C = SDL_SCANCODE_C,
  B = SDL_SCANCODE_B,
  N = SDL_SCANCODE_N,
  M = SDL_SCANCODE_M,
  F = SDL_SCANCODE_F,
  G = SDL_SCANCODE_G,
  H = SDL_SCANCODE_H,
  R = SDL_SCANCODE_R,
  T = SDL_SCANCODE_T,
  Y = SDL_SCANCODE_Y,
  LARROW = SDL_SCANCODE_LEFT,
  RARROW = SDL_SCANCODE_RIGHT,
  UARROW = SDL_SCANCODE_UP,
  DARROW = SDL_SCANCODE_DOWN,
} Keys;

void audio_callback(void *userdata, Uint8 *stream, int length);

class InputMap;
class SynthWrapper;
class ADSRGen;
class Synth;
class FDN;
typedef double (SynthWrapper::*wave_fn_ptr)(double, double);

class FDN {
public:
  FDN();
  ~FDN();

private:
};

class InputMap {
public:
  InputMap();
  ~InputMap();
  void up_octave(double frequency[]);
  void down_octave(double frequency[]);
  int find_held_key(int SCANCODE);
  void synth_key_pressed(int index, int playing[]);
  void ctrls_key_pressed(int SCANCODE, int *wave_ptr_index, double frequency[]);
  void ctrls_key_released(int SCANCODE);
  void key_up(int SCANCODE, int playing[]);
  void define_keymaps();
  void setup_inputs();
  void synth_key_released(int index, int playing[]);
  void key_down(int SCANCODE, int playing[], int *wave_ptr_index,
                double frequency[]);
  void previous_wave_fn(int *wave_ptr_index);
  void next_wave_fn(int *wave_ptr_index);
  void poll_events(Synth *syn, SynthWrapper *synfunc);
  std::map<int, std::pair<int, int>> *KM;
  std::set<int> held_keys;

private:
};

class SynthWrapper {
public:
  SynthWrapper();
  ~SynthWrapper();
  wave_fn_ptr ptr_arr[3];
  double modulator(double freq, double mi, double dtime);
  double pulse_wave(double freq, double dtime);
  int wave_ptr_index;
  double distort(double in, double amount);
  double w(double freq);
  double sawtooth(double freq, double time);
  double sine(double freq, double time);
  double square(double freq, double time);
  double triangle(double freq, double time);
  void create_synth_ptrs();
  double call_func(double freq, double dtime);

private:
  double time_scalar;
};

class Synth {
public:
  Synth(InputMap *inputs, SynthWrapper *synfunc);
  ~Synth();
  void do_render();
  int init_audio();
  void set_default_buffer();
  void create_default_settings();
  int create_window();
  int create_renderer();
  void setup_frequencies();
  void run_main_loop(InputMap *inputs, SynthWrapper *synfunc);
  void generate_samples(SynthWrapper *synfunc);
  void check_play_state(int *playing_flag);
  void create_sample_radians(SynthWrapper *synfunc, double *sam, double *ssum);
  double generate_envelope(SynthWrapper *synfunc);
  double get_max_value(double absmax, double sample);
  double normalize_sample(double sample_sum, double absmax);
  double handle_release(int n);
  double create_envelope(SynthWrapper *synfunc, int n, double package);
  double generate_wave(SynthWrapper *synfunc, int n, double t);
  double tempo;
  double *times;
  double *time_periods;
  int buffer_flag;
  int render_flag;
  int *playing;
  double *frequency;
  Sint16 BUFFER_DATA[BUFFERSIZE];
  int running;
  double samples[BUFFERSIZE];
  double beat_duration;
  double note_duration;
  double bar_time;

private:
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioSpec spec;
  SDL_AudioDeviceID device;
  int startup_flag;
};

class DataOps {
public:
  double *downsample(double filtered_buffer[], int buffer_length, int dfactor);
  double *generate_coefficients(int filter_length, double beta);
  double *FIRfunc(double buffer[], double coeff[], int filter_length);
  double *kaiser_window(double beta, int filter_length);
  void swapf(double *a, double *b);
  int partitionf(double *map, int low, int high);
  void quicksortf(double *map, int low, int high);
  void swap(int *a, int *b);
  int partition(int *map, int low, int high);
  void quicksort(int *map, int low, int high);
  int find_key_pressed(int *enumlist, int target);
};

class ADSRGen {
public:
  ADSRGen();
  ~ADSRGen();
  void set_attack_env(int n, double time);
  void set_decay_env(int n, double time);
  void set_sustain_env(int n);
  void set_release_env(int n);
  void set_gate_switch(int n, double time);
  void handle_envelope_generation(int n, double time);
  double AT;
  double DT;
  double SL;
  double RT;
  double envelope[NOTES];
  int ads_on;

private:
};
