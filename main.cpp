#include "inc/main.hpp"

int main() {
  {
    Synth synth;
    synth.run_main_loop();
  }
  return 0;
}

void Synth::run_main_loop() {
  while (this->running == 1) {
    this->poll_events();
  }
}
