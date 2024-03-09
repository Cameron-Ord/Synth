#include "inc/main.hpp"
#include "inc/scancodes.hpp"
#include <functional>
#include <utility>

Synth::Synth(Initializer *init) {
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

  std::set<std::pair<double, int>> freq_km_sets;
  std::function<void()> note_function =
      std::bind(&Synth::handle_synth_key, this);
  for (size_t i = 0; i < notes_len; i++) {
    double note = base_freqs[i];
    int scancode = base_km[i];
    frequencies.emplace(std::make_pair(note, 0), std::make_pair(0.0, 0.0));
    freq_km_sets.insert(std::make_pair(note, scancode));
  }
  KM.emplace(freq_km_sets, note_function);
}

void Synth::set_buffers() {
  double DBUFFER[BL];
  int16_t SBUFFER[BL];
  buffers = std::make_pair(DBUFFER, SBUFFER);
}

void Synth::set_params() {
  std::pair<std::string, double> tempo = std::make_pair("tempo", 120.0);

  std::pair<std::string, double> beat_duration =
      std::make_pair("beat_duration", (60.0 / tempo.second));

  std::pair<std::string, double> note_duration =
      std::make_pair("note_duration", (beat_duration.second / 8));

  std::pair<std::string, double> time_step =
      std::make_pair("time_step", 1.0 / SR);

  playback_params.insert(tempo);
  playback_params.insert(beat_duration);
  playback_params.insert(note_duration);
  playback_params.insert(time_step);

  printf("-----------\n");
  printf("TEMPO : %f, BEAT : %f, NOTE : %f TIME_STEP : %f\n", tempo.second,
         beat_duration.second, note_duration.second, time_step.second);
  printf("-----------\n");
}

void Synth::run_synth(Initializer *init) {}

std::set<std::pair<std::string, double>> Synth::get_params() {
  return playback_params;
}

std::pair<double *, int16_t *> Synth::get_buffers() { return buffers; }
