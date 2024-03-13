#include "inc/main.hpp"

int main() {
  std::set<int> err;
  SDL_State*    init   = new SDL_State(&err);
  Inputs*       inputs = new Inputs();
  Synth*        synth  = new Synth();
  Renderer*     rend   = new Renderer();
  synth->set_defaults(inputs->get_base_km_ptr(), inputs->get_alt_km_ptr());
  init->create_spec(synth);
  init->create_dev(&err);
  init->open_audio();
  auto iter = err.find(-1);
  if (iter != err.end()) {
    printf("SDL FAILED TO INIT.\n");
    cleanup(init, inputs, synth, rend);
    SDL_Quit();
    return 1;
  }

  synth->set_run_state(1);
  while (*synth->get_run_state() == 1) {
    inputs->poll_events(synth, rend, init);
    synth->create_sample_buffer();
    rend->do_render(init, synth);
    SDL_Delay(16);
  }
  cleanup(init, inputs, synth, rend);
  SDL_Quit();
  return 0;
}

void cleanup(SDL_State* st, Inputs* in, Synth* syn, Renderer* ren) {
  delete ren;
  delete in;
  delete syn;
  delete st;
};
