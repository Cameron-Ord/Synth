#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>

typedef enum {
  A = SDLK_a,
  S = SDLK_s,
  D = SDLK_d,
  F = SDLK_f,
  H = SDLK_h,
  J = SDLK_j,
  K = SDLK_k,
  L = SDLK_l,
  E = SDLK_e,
  W = SDLK_w,
  U = SDLK_u,
  I = SDLK_i
} NOTE_KEYS;

typedef enum {
  RESIZED = SDL_WINDOWEVENT_RESIZED,
} WIN_EVENTS;

typedef enum {

} CONFIG_KEYS;
