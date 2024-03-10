#include "inc/main.hpp"
#include "inc/scancodes.hpp"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <utility>

Synth::Synth(Initializer *init) {
  running = 0;
  add_spec_values(init->get_spec());
  set_frequencies();
  set_params();
  set_buffers();
  run_synth(init);
}

void Synth::add_spec_values(SDL_AudioSpec *spec) {
  spec->userdata = this;
  spec->callback = audio_callback;
}

void Synth::set_frequencies() {
  base_freqs = {
      130.81, 138.59, 146.83, 155.56, 164.81, 174.61,
      185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
  };
  base_km = {A, S, D, F, W, E, H, J, K, L, U, I};
  notes_len = base_freqs.size();
  for (size_t i = 0; i < notes_len; i++) {
    int KEY = base_km[i];
    double NOTE = base_freqs[i];
    frequencies.emplace(KEY, std::make_pair(NOTE, 0));
  }
  printf("\n\n");
  for (size_t j = 0; j < notes_len / 2; j++) {
    printf("KEY : %c -> NOTE : %f | KEY : %c -> NOTE : %f\n",
           *SDL_GetKeyName(base_km[j]), base_freqs[j],
           *SDL_GetKeyName(base_km[j + 1]), base_freqs[j + 1]);
  }
}

void Synth::set_buffers() {
  double DBUFFER[BL];
  int16_t SBUFFER[BL];
  buffers = std::make_pair(DBUFFER, SBUFFER);
}

void Synth::set_params() {
  params.emplace("tempo", 120.0);
  params.emplace("beat_duration", (60.0 / params["tempo"]));
  params.emplace("note_duration", (params["beat_duration"] / 8));
  params.emplace("time_step", (1.0 / SR));
  printf("---------------------------------------------------------------------"
         "---------------------\n");
  printf("TEMPO : %f, BEAT DURATION : %f, NOTE DURATION : %f, TIME STEP : %f\n",
         params["tempo"], params["beat_duration"], params["note_duration"],
         params["time_step"]);
  printf("---------------------------------------------------------------------"
         "---------------------\n");
}

std::map<std::string, double> Synth::get_params() { return params; }

std::pair<double *, int16_t *> Synth::get_buffers() { return buffers; }

void Synth::run_synth(Initializer *init) {
  running = 1;
  while (running) {
    poll_events();
  }
}
