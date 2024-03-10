#include "inc/main.hpp"
#include "inc/scancodes.hpp"
#include <stk/Fir.h>
#include <stk/Stk.h>

Synth::Synth() {
  buffer_enabled = 0;
  running = 0;
  set_frequencies();
  set_params();
  set_buffers();
  set_fir_filter();
  set_adsr();
  set_freeverb();
}

Synth::~Synth() {
  delete[] buffers.first;
  delete[] buffers.second;
  delete playing_freqs;
  delete frequencies;
}

void Synth::set_fir_filter() {
  filt_len = 200;
  coeffs = generate_coefficients(filt_len, 7.5);
  std::vector<stk::StkFloat> coeff_vector;
  for (int i = 0; i < filt_len; i++) {
    coeff_vector.push_back(static_cast<stk::StkFloat>(coeffs[i]));
  }
  filter.setCoefficients(coeff_vector);
  delete[] coeffs;
}

void Synth::set_adsr() {
  stk::StkFloat attack = 0.1;
  stk::StkFloat decay = 0.2;
  stk::StkFloat sustain = 0.8;
  stk::StkFloat release = 0.3;
  adsr.setAttackRate(attack);
  adsr.setDecayRate(decay);
  adsr.setSustainLevel(sustain);
  adsr.setReleaseRate(release);
}

void Synth::set_freeverb() {
  verb.setEffectMix(0.2);
  verb.setRoomSize(0.4);
  verb.setDamping(0.3);
}
void Synth::set_frequencies() {
  base_freqs = {
      130.81, 138.59, 146.83, 155.56, 164.81, 174.61,
      185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
  };
  base_km = {A, S, D, F, W, E, H, J, K, L, U, I};
  notes_len = base_freqs.size();

  frequencies = new std::map<int, std::pair<double, double>>;
  for (size_t i = 0; i < notes_len; i++) {
    int KEY = base_km[i];
    double NOTE = base_freqs[i];
    frequencies->emplace(KEY, std::make_pair(NOTE, 0.0));
  }
  printf("\n\n");
  for (size_t j = 0; j < notes_len / 2; j++) {
    printf("KEY : %c -> NOTE : %f | KEY : %c -> NOTE : %f\n",
           *SDL_GetKeyName(base_km[j]), base_freqs[j],
           *SDL_GetKeyName(base_km[j + 1]), base_freqs[j + 1]);
  }
  playing_freqs = new std::vector<std::pair<double, double *>>;
}

void Synth::set_buffers() {
  double *DBUFFER = new double[BL];
  int16_t *SBUFFER = new int16_t[BL];
  // setting both buffers to 0 to on start.
  for (int i = 0; i < BL; i++) {
    DBUFFER[i] = 0.0;
    SBUFFER[i] = 0;
  }
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

int Synth::get_enabled_state() { return buffer_enabled; }
int *Synth::get_run_state() { return &running; }
std::map<std::string, double> Synth::get_params() { return params; }

std::pair<double *, int16_t *> Synth::get_buffers() { return buffers; }
