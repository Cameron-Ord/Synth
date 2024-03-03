#include "inc/main.hpp"

void Synth::do_render() {
  float cell_width = static_cast<float>(BWIDTH) / BUFFERSIZE;
  float heightf = static_cast<float>(BHEIGHT);
  if (this->render_flag == 1) {
    SDL_SetRenderDrawColor(this->r, 40, 42, 54, 0);
    SDL_RenderClear(this->r);
    for (int i = 0; i < BUFFERSIZE; i++) {
      if (this->BUFFER_DATA[i] > 0) {

        float t = static_cast<float>(this->BUFFER_DATA[i]) / INT16_MAX;
        int x_pos = i * cell_width;
        int y_pos = heightf / 2 - heightf / 2 * t;
        int rect_w = ceilf(cell_width);
        int rect_h = heightf / 2 * t;

        SDL_Rect radian_rect = {x_pos, static_cast<int>(y_pos), rect_w,
                                static_cast<int>(rect_h)};

        SDL_SetRenderDrawColor(this->r, 189, 147, 249, 0);
        SDL_RenderFillRect(this->r, &radian_rect);

      } else if (this->BUFFER_DATA[i] < 0) {

        float t = static_cast<float>(this->BUFFER_DATA[i]) / INT16_MIN;
        int x_pos = i * cell_width;
        int y_pos = heightf / 2;
        int rect_w = ceilf(cell_width);
        int rect_h = heightf / 2 * t;

        SDL_Rect radian_rect = {x_pos, y_pos, rect_w, rect_h};

        SDL_SetRenderDrawColor(this->r, 189, 147, 249, 255);
        SDL_RenderFillRect(this->r, &radian_rect);
      }
    }
  }
  SDL_RenderPresent(this->r);
}
