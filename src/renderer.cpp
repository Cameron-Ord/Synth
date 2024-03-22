#include "inc/main.hpp"

Renderer::Renderer() {
  win_width  = BWIDTH;
  win_height = BHEIGHT;
  cell_width = static_cast<float>(win_width) / BL;
  heightf    = static_cast<float>(win_height);
}

void Renderer::update_render_values() {
  cell_width = static_cast<float>(win_width) / BL;
  heightf    = static_cast<float>(win_height);
}

void Renderer::apply_win_resize(SDL_State* init) {
  SDL_GetWindowSize(init->get_window(), &win_width, &win_height);
  update_render_values();
}

void Renderer::do_render(SDL_State* init, Synth* syn) {
  SDL_SetRenderDrawColor(init->get_renderer(), 40, 42, 54, 0);
  SDL_RenderClear(init->get_renderer());
  double*  fbuff = syn->get_left_buffer();
  int16_t* sbuff = syn->get_sbuffer();

  if (syn->get_enabled_state()) {
    for (int i = 0; i < BL; i++) {
      if (sbuff[i] > 0) {
        float    t           = fbuff[i] / INT16_MAX;
        int      x_pos       = i * cell_width;
        int      y_pos       = static_cast<int>(heightf / 2 - heightf / 2 * t);
        int      rect_w      = ceilf(cell_width);
        int      rect_h      = static_cast<int>(heightf / 2 * t);
        SDL_Rect radian_rect = {x_pos, y_pos, rect_w, rect_h};
        SDL_SetRenderDrawColor(init->get_renderer(), 189, 147, 249, 0);
        SDL_RenderFillRect(init->get_renderer(), &radian_rect);
      } else if (sbuff[i] < 0) {
        float    t           = fbuff[i] / INT16_MIN;
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
