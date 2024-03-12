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
  I = SDLK_i,
  Q = SDLK_q,
  R = SDLK_r,
  Y = SDLK_y,
  O = SDLK_o,
  Z = SDLK_z,
  X = SDLK_x,
  C = SDLK_c,
  B = SDLK_b,
  N = SDLK_n,
  M = SDLK_m,
  T = SDLK_t,
  P = SDLK_p,
} NOTE_KEYS;

typedef enum {
  RESIZED = SDL_WINDOWEVENT_RESIZED,
} WIN_EVENTS;

typedef enum {

} CONFIG_KEYS;
