#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>

#include <cstdlib>
#include <cstring>

#define HEIGHT 600
#define WIDTH 800
#define NOTES 12
#define SAMPLERATE 44100
#define BUFFERSIZE 256

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
} Keys;

void audio_callback(void *userdata, Uint8 *stream, int length);

class Synth {
public:
  Synth();
  ~Synth();

  int init_audio();
  void define_keymaps();
  void run_main_loop();
  void poll_events();
  void setup_inputs();
  void key_down(int SCANCODE);
  void key_up(int SCANCODE);
  int create_window();
  int create_renderer();
  int *playing;
  double *frequency;
  int *KM;

private:
  SDL_Window *w;
  SDL_Renderer *r;
  SDL_AudioSpec spec;
  SDL_AudioDeviceID device;
  int running;
  int startup_flag;
};
