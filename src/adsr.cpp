#include "inc/main.hpp"

double Synth::handle_envelope_gen(double time) {
  if (time < adsr->AT) {
    double envelope = set_attack_env(time);
    return envelope;
  } else if (time < adsr->AT + adsr->DT) {
    double envelope = set_decay_env(time);
    return envelope;
  } else {
    double envelope = set_sustain_env();
    return envelope;
  }
  return 0.0;
}

double Synth::set_attack_env(double time) {
  double envelope = time / adsr->AT;
  return envelope;
}

double Synth::set_decay_env(double time) {
  double envelope = 1.0 - (1.0 - adsr->SL) * ((time - adsr->AT) / adsr->DT);
  return envelope;
}

double Synth::set_sustain_env() {
  double envelope = adsr->SL;
  return envelope;
}
