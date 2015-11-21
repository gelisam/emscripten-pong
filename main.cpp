#include <assert.h>
#include <emscripten.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>


SDL_Surface *screen;
const int WIDTH = 800;
const int HEIGHT = 600;


typedef Uint32 Color;

// Color red = rgb(255,0,0);
inline Color rgb(Uint8 r, Uint8 g, Uint8 b) {
  return SDL_MapRGB(screen->format, r, g, b);
}

// Color red = rgb(0xff0000);
inline Color rgb(unsigned int rrggbb) {
  int r = (rrggbb & 0xff0000) >> 16;
  int g = (rrggbb & 0x00ff00) >> 8;
  int b = (rrggbb & 0x0000ff);
  return rgb(r, g, b);
}

Color current_fg = rgb(0xffffff);
Color current_bg = rgb(0x000000);


double t_at_start, t_at_last_frame, t, dt;


struct GameState {
  double left_paddle_y = 0.5;
  double right_paddle_y = 0.5;
  
  double ball_x = 0.5;
  double ball_y = 0.5;
  double ball_dx = 0.5;
  double ball_dy = 0.5;
} game_state;


void handle_event(const SDL_Event& event) {
  if(event.key.keysym.sym == SDLK_c) {
    printf("switch_color\n");
    current_fg = rgb(0x0000ff);
  }
}

GameState advance_simulation(const GameState& g) {
  GameState gg = g;
  
  gg.ball_x = g.ball_x + g.ball_dx;
  gg.ball_y = g.ball_y + g.ball_dy;
  
  return gg;
}


inline SDL_Rect rect(double x, double y, double w, double h) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

inline SDL_Rect square(double x, double y, double radius=5) {
  return rect(x-radius, y-radius, 2*radius, 2*radius);
}


inline void clear(Color color=current_bg) {
  assert(SDL_FillRect(screen, NULL, color) == 0);
}

void draw(const SDL_Rect& r, Color color=current_fg) {
  assert(SDL_FillRect(screen, &r, color) == 0);
}

void draw_frame() {
  clear();
  draw(square(game_state.ball_x, game_state.ball_y));
}


void main_loop() {
  t = emscripten_get_now() - t_at_start;
  dt = t - t_at_last_frame;
  t_at_last_frame = t;
  
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    handle_event(event);
  }
  
  game_state = advance_simulation(game_state);
  
  draw_frame();
  SDL_Flip(screen);
}
 
int main() {
  assert(SDL_Init(SDL_INIT_VIDEO) >= 0);
  
  screen=SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
  assert(screen);
  
  t_at_start = emscripten_get_now();
  emscripten_set_main_loop(main_loop, 0, 0);
  
  return 0;
}
