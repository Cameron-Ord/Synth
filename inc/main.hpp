#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>

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
} Keys;

void audio_callback(void *userdata, Uint8 *stream, int length);

class Synth {
public:
  Synth();
  ~Synth();
  void do_render();
  int init_audio();
  void define_keymaps();
  void run_main_loop();
  void poll_events();
  void setup_inputs();
  void key_down(int SCANCODE);
  void key_up(int SCANCODE);
  int create_window();
  int create_renderer();
  void set_default_buffer();
  void create_default_settings();
  int render_flag;
  int *playing;
  double *frequency;
  int *KM;
  Sint16 BUFFER_DATA[BUFFERSIZE];

  double note_notation;
  int tempo;
  double t;
  double AT;
  double DT;
  double SL;
  double RT;

private:
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioSpec spec;
  SDL_AudioDeviceID device;
  int running;
  int startup_flag;
};
