#include "synths.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>
#include <map>
#define BHEIGHT 800
#define BWIDTH 1600
#define NOTES 24
#define SAMPLERATE 44100
#define BUFFERSIZE 512
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
} Keys;

void audio_callback(void *userdata, Uint8 *stream, int length);

class Synth {
public:
  Synth();
  ~Synth();

  typedef double (*wave_fn_ptr)(double, double);
  wave_fn_ptr ptr_arr[6];
  int wave_ptr_index;
  void do_render();
  int init_audio();
  void define_keymaps();
  void run_main_loop();
  void poll_events();
  void setup_inputs();
  void key_down(int SCANCODE);
  void synth_key_pressed(int index);
  void ctrls_key_pressed(int SCANCODE);
  void synth_key_released(int index);
  void ctrls_key_released(int SCANCODE);
  void key_up(int SCANCODE);
  int create_window();
  int create_renderer();
  void set_default_buffer();
  void create_default_settings();
  void create_synth_ptrs();
  int render_flag;
  int *playing;
  double *frequency;
  Sint16 BUFFER_DATA[BUFFERSIZE];
  std::map<int, std::pair<int, int>> *KM;
  double note_notation;
  int tempo;
  double t;
  double AT;
  double DT;
  double SL;
  double RT;

private:
  void previous_wave_fn();
  void next_wave_fn();
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioSpec spec;
  SDL_AudioDeviceID device;
  int running;
  int startup_flag;
};
