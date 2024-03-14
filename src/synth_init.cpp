#include "inc/main.hpp"
#include "inc/scancodes.hpp"
Synth::Synth() {
  buffer_enabled = 0;
  running        = 0;
  set_buffers();
  set_adsr();
}

Synth::~Synth() {
  delete[] fbuffer;
  delete[] sbuffer;
  delete freq_map;
  delete adsr;
  delete samples;
}

void Synth::set_adsr() {
  adsr     = new ADSR_PARAMS;
  adsr->AT = 0.1;
  adsr->DT = 0.4;
  adsr->SL = 0.8;
  adsr->RT = 0.6;
}

void Synth::set_buffers() {
  samples = new double[NOTELEN * 2];
  memset(samples, 0, sizeof(double) * NOTELEN);
  fbuffer = new double[BL];
  sbuffer = new int16_t[BL];
  for (int i = 0; i < BL; i++) {
    fbuffer[i] = 0.0;
    sbuffer[i] = 0;
  }
}

void Synth::set_defaults(std::vector<int>* base_km, std::vector<int>* alt_km) {
  t          = 1.0 / SR;
  base_freqs = {
      130.81, 138.59, 146.83, 155.56, 164.81, 174.61,
      185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
  };
  *base_km  = {A, S, D, F, W, E, H, J, K, L, U, I};
  *alt_km   = {Q, R, T, Z, X, C, B, N, M, Y, O, P};
  notes_len = base_freqs.size();
  freq_map  = new std::map<int, Freq_Data>;
  for (size_t i = 0; i < notes_len; i++) {
    Freq_Data fd;
    int       KEY  = (*base_km)[i];
    double    NOTE = base_freqs[i] * pow(2.0, 1);
    fd.freq        = NOTE;
    fd.is_dead     = 1;
    fd.time        = 0.0;
    freq_map->emplace(KEY, fd);
  }
  for (int i = 0; i < notes_len; i++) {
    Freq_Data fd;
    int       KEY  = (*alt_km)[i];
    double    NOTE = base_freqs[i];
    fd.freq        = NOTE;
    fd.is_dead     = 1;
    fd.time        = 0.0;
    freq_map->emplace(KEY, fd);
  }
}

std::map<int, Freq_Data>* Synth::get_freq_map() { return freq_map; }
double*                   Synth::get_fbuffer() { return fbuffer; }
int16_t*                  Synth::get_sbuffer() { return sbuffer; }
int*                      Synth::get_run_state() { return &running; }
int                       Synth::get_enabled_state() { return buffer_enabled; }
void                      Synth::set_run_state(int r) { running = r; }
void                      Synth::set_enabled_state(int en) { buffer_enabled = en; }
