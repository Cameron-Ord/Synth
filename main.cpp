#include "inc/main.hpp"

int main() {

  std::set<int> err;
  Initializer *init = new Initializer(&err);
  auto iter = err.find(-1);
  if (iter != err.end()) {
    printf("---------------\n");
    printf("SDL FAILED TO INITIALIZE, PERFORMING CLEANUP AND EXITING\n");
    printf("---------------\n");
    delete init;
    SDL_Quit();
    return 1;
  }

  Synth *synth = new Synth(init);
  SDL_Quit();
  return 0;
}
