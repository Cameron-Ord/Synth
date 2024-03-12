#include "inc/main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

Renderer::Renderer() {
  win_height = BHEIGHT;
  win_width  = BWIDTH;
  cell_width = static_cast<float>(win_width) / BL;
  heightf    = static_cast<float>(win_height);
}

void Renderer::update_render_values() {
  cell_width = static_cast<float>(win_width) / BL;
  heightf    = static_cast<float>(win_height);
}

void Renderer::apply_win_resize(Initializer* init) {
  SDL_GetWindowSize(init->get_window(), &win_width, &win_height);
  update_render_values();
}

void Renderer::do_render(Initializer* init, Synth* syn) {
  SDL_SetRenderDrawColor(init->get_renderer(), 40, 42, 54, 0);
  SDL_RenderClear(init->get_renderer());
  std::pair<double*, int16_t*> b = syn->get_buffers();

  if (syn->get_enabled_state()) {
    for (int i = 0; i < BL; i++) {
      if (b.second[i] > 0) {
        float    t           = b.first[i] / INT16_MAX;
        int      x_pos       = i * cell_width;
        int      y_pos       = static_cast<int>(heightf / 2 - heightf / 2 * t);
        int      rect_w      = ceilf(cell_width);
        int      rect_h      = static_cast<int>(heightf / 2 * t);
        SDL_Rect radian_rect = {x_pos, y_pos, rect_w, rect_h};
        SDL_SetRenderDrawColor(init->get_renderer(), 189, 147, 249, 0);
        SDL_RenderFillRect(init->get_renderer(), &radian_rect);
      } else if (b.second[i] < 0) {
        float    t           = b.first[i] / INT16_MIN;
        int      x_pos       = i * cell_width;
        int      y_pos       = static_cast<int>(heightf / 2);
        int      rect_w      = ceilf(cell_width);
        int      rect_h      = static_cast<int>(heightf / 2 * t);
        SDL_Rect radian_rect = {x_pos, y_pos, rect_w, rect_h};
        SDL_SetRenderDrawColor(init->get_renderer(), 189, 147, 249, 255);
        SDL_RenderFillRect(init->get_renderer(), &radian_rect);
      }
    }
  }
  SDL_RenderPresent(init->get_renderer());
}
