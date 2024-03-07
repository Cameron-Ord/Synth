#include "inc/main.hpp"

InputMap::InputMap() { this->define_keymaps(); }

void InputMap::define_keymaps() {
  int SYNTH_KEYMAPS[] = {Q, W, E, A, S, D, J, K, L, U, I, O,
                         Z, X, C, B, N, M, F, G, H, R, T, Y};

  int CONTROLS_KEYMAPS[] = {LARROW, RARROW, UARROW, DARROW};
  this->KM = new std::map<int, std::pair<int, int>>;
  for (int i = 0; i < NOTES; i++) {
    std::pair<int, int> index_and_call = std::make_pair(i, 1);
    this->KM->emplace(std::make_pair(SYNTH_KEYMAPS[i], index_and_call));
  }

  int LEN = sizeof(CONTROLS_KEYMAPS) / sizeof(CONTROLS_KEYMAPS[0]);
  for (int c = 0; c < LEN; c++) {
    std::pair<int, int> index_and_call = std::make_pair(c, 2);
    this->KM->emplace(std::make_pair(CONTROLS_KEYMAPS[c], index_and_call));
  }
}

InputMap::~InputMap() { delete this->KM; }

SynthWrapper::SynthWrapper() {
  this->create_synth_ptrs();
  this->wave_ptr_index = 0;
}

SynthWrapper::~SynthWrapper() {}

Synth::Synth(InputMap *inputs, SynthWrapper *synfunc) {
  this->setup_frequencies();
  this->set_default_buffer();
  this->create_default_settings();
  this->running = 0;
  this->startup_flag = 0;
  this->render_flag = 0;
  this->buffer_flag = 0;
  int err;

  this->w = NULL;
  err = this->create_window();
  if (err < 0) {
    startup_flag = -1;
  }

  this->r = NULL;
  err = this->create_renderer();
  if (err < 0) {
    startup_flag = -1;
  }

  err = this->init_audio();
  if (err < 0) {
    startup_flag = -1;
  }

  if (this->startup_flag == 0) {
    SDL_PauseAudioDevice(this->device, 0);
    SDL_PauseAudio(0);
    this->running = 1;
    this->render_flag = 1;
    this->run_main_loop(inputs, synfunc);
  }
}

Synth::~Synth() {
  if (this->r) {
    SDL_DestroyRenderer(this->r);
  }
  if (this->w) {
    SDL_DestroyWindow(this->w);
  }
  if (this->device) {
    SDL_CloseAudioDevice(this->device);
  }
  SDL_CloseAudio();
  delete[] this->frequency;
  delete[] this->playing;
  delete[] this->times;
  delete[] this->time_periods;
}

void Synth::create_default_settings() {
  this->tempo =  120.0;
}

void Synth::set_default_buffer() {
  memset(this->BUFFER_DATA, 0, sizeof(Sint16) * BUFFERSIZE);
}

void Synth::setup_frequencies() {
  double f[] = {130.81,     138.59,     146.83,     155.56,     164.81,
                174.61,     185.00,     196.00,     207.65,     220.00,
                233.08,     246.94,     130.81 * 2, 138.59 * 2, 146.83 * 2,
                155.56 * 2, 164.81 * 2, 174.61 * 2, 185.00 * 2, 196.00 * 2,
                207.65 * 2, 220.00 * 2, 233.08 * 2, 246.94 * 2};

  this->frequency = new double[NOTES];
  this->playing = new int[NOTES];
  this->time_periods = new double[NOTES];
  this->times = new double[NOTES];

  for (int k = 0; k < NOTES; k++) {
    this->frequency[k] = f[k];
    this->time_periods[k] = (1.0 / f[k]);
    this->times[k] = 0.0;
    this->playing[k] = 0;
  }
}

int Synth::create_window() {
  this->w = SDL_CreateWindow("Synth", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, BWIDTH, BHEIGHT, 0);
  if (!this->w) {
    SDL_Log("Failed to create window! %s", SDL_GetError());
    return -1;
  }

  SDL_SetWindowResizable(this->w, SDL_FALSE);
  return 0;
}

int Synth::create_renderer() {
  this->r = SDL_CreateRenderer(
      this->w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!this->r) {
    SDL_Log("Failed to create renderer! %s", SDL_GetError());
    SDL_DestroyWindow(this->w);
    return -1;
  }
  return 0;
}

int Synth::init_audio() {
  this->spec.freq = SAMPLERATE;
  this->spec.format = AUDIO_S16;
  this->spec.channels = 1;
  this->spec.samples = BUFFERSIZE;
  this->spec.callback = audio_callback;
  this->spec.userdata = this;
  this->device = SDL_OpenAudioDevice(NULL, 0, &this->spec, NULL, 0);
  if (!this->device) {
    SDL_Log("Failed to open audio device %s", SDL_GetError());
    return -1;
  }
  return 0;
}
