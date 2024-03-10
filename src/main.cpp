#include "inc/main.hpp"

int main() {

  std::set<int> err;

  Synth *synth = new Synth();
  Initializer *init = new Initializer(&err, synth);
  auto iter = err.find(-1);
  if (iter != err.end()) {
    printf("---------------\n");
    printf("SDL FAILED TO INITIALIZE, PERFORMING CLEANUP AND EXITING\n");
    printf("---------------\n");
    delete synth;
    delete init;
    SDL_Quit();
    return 1;
  }

  delete synth;
  delete init;
  SDL_Quit();
  return 0;
}
