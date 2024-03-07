#include "inc/main.hpp"

ADSRGen::ADSRGen() {
  this->AT = 0.01;
  this->DT = 0.1;
  this->SL = 0.8;
  this->RT = 0.1;
  memset(this->envelope, 0, sizeof(double) * NOTES);
}

ADSRGen::~ADSRGen() {}

void ADSRGen::handle_envelope_generation(int n, double time) {
  if (time < this->AT) {
    this->set_attack_env(n, time);
  } else if (time < this->AT + this->DT) {
    this->set_decay_env(n, time);
  } else {
    this->set_sustain_env(n);
  }
}

void ADSRGen::set_attack_env(int n, double time) {
  this->envelope[n] = time / this->AT;
}

void ADSRGen::set_decay_env(int n, double time) {
  this->envelope[n] = 1.0 - (1.0 - this->SL) * ((time - this->AT) / this->DT);
}

void ADSRGen::set_sustain_env(int n) { this->envelope[n] = this->SL; }

void ADSRGen::set_release_env(int n) { this->envelope[n] = this->RT; }
