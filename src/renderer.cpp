#include "inc/main.hpp"

float cell_width = static_cast<float>(BWIDTH) / BL;
float heightf = static_cast<float>(BHEIGHT);
void Renderer::do_render(Initializer *init, Synth *syn) {
  SDL_SetRenderDrawColor(init->get_renderer(), 40, 42, 54, 0);
  SDL_RenderClear(init->get_renderer());
  if (syn->get_enabled_state()) {
  }
  SDL_RenderPresent(init->get_renderer());
}
