#include "inc/main.hpp"

void cleanup(Synth* s, Initializer* i, Renderer* r, Inputs* in) {
  delete in;
  delete r;
  delete s;
  delete i;
  SDL_Quit();
}

int main() {

  std::set<int> err;
  Renderer*     rend   = new Renderer();
  Inputs*       inputs = new Inputs();
  Synth*        synth  = new Synth();
  Initializer*  init   = new Initializer(&err);
  synth->set_frequencies(inputs->get_base_km_ptr(), inputs->get_alt_km_ptr());
  init->create_spec(synth);
  init->create_dev(&err);
  init->open_audio();
  auto iter = err.find(-1);
  if (iter != err.end()) {
    printf("SDL FAILED TO INITIALIZE, PERFORMING CLEANUP AND EXITING\n");
    cleanup(synth, init, rend, inputs);
    return 1;
  }

  synth->set_run_state(1);
  while (*synth->get_run_state() == 1) {
    inputs->poll_events(synth, rend, init);
    synth->create_sample_buffer();
    rend->do_render(init, synth);
    SDL_Delay(16);
  }

  delete inputs;
  delete rend;
  delete synth;
  delete init;
  SDL_Quit();
  return 0;
}
