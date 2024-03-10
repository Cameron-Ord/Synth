#include "inc/main.hpp"

double Synth::handle_envelope_gen(double time) {
  if (time < AT) {
    double envelope = set_attack_env(time);
    return envelope;
  } else if (time < AT + DT) {
    double envelope = set_decay_env(time);
    return envelope;
  } else {
    double envelope = set_sustain_env();
    return envelope;
  }
  return 0.0;
}

double Synth::set_attack_env(double time) {
  double envelope = time / AT;
  return envelope;
}

double Synth::set_decay_env(double time) {
  double envelope = 1.0 - (1.0 - SL) * ((time - AT) / DT);
  return envelope;
}

double Synth::set_sustain_env() {
  double envelope = SL;
  return envelope;
}
