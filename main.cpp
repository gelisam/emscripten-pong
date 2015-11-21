#include <math.h>
#include "engine.cpp"


double ball_radius = 0.02;
V2 paddle_size = V2(0.03, 0.2);

Color ball_color;
Color paddle_color;
double paddle_speed = 0.05;

void init() {
  ball_color = rgb(0xccffcc);
  paddle_color = rgb(0x88aa88);
}


struct GameState {
  double left_paddle_y = 0.0;
  
  bool key_up_is_pressed = false;
  bool key_down_is_pressed = false;
  
  V2 ball_pos = screen_center;
  V2 ball_speed = V2(0.01, 0.01);
} game_state;

inline Rect ball_rect(const GameState& g=game_state) {
  return square_at(ball_radius*2, g.ball_pos);
}

inline Rect left_paddle_rect(const GameState& g=game_state) {
  return rect_at(paddle_size, screen_left + V2(0.03, g.left_paddle_y));
}


void handle_event(const SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
        case SDLK_UP:   case SDLK_w: game_state.key_up_is_pressed = true; break;
        case SDLK_DOWN: case SDLK_s: game_state.key_down_is_pressed = true; break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
        case SDLK_UP:   case SDLK_w: game_state.key_up_is_pressed = false; break;
        case SDLK_DOWN: case SDLK_s: game_state.key_down_is_pressed = false; break;
      }
      break;
  }
}

GameState next(const GameState& g);

GameState bounce_right(const GameState& g) {
  GameState gg = g;
  gg.ball_speed.x = fabs(gg.ball_speed.x);
  return next(gg);
}

GameState bounce_left(const GameState& g) {
  GameState gg = g;
  gg.ball_speed.x = -fabs(gg.ball_speed.x);
  return next(gg);
}

GameState bounce_up(const GameState& g) {
  GameState gg = g;
  gg.ball_speed.y = fabs(gg.ball_speed.y);
  return next(gg);
}

GameState bounce_down(const GameState& g) {
  GameState gg = g;
  gg.ball_speed.y = -fabs(gg.ball_speed.y);
  return next(gg);
}

GameState next(const GameState& g) {
  GameState gg = g;
  
  gg.ball_pos = g.ball_pos + g.ball_speed;
  
  if (collides(ball_rect(gg), left_paddle_rect(gg))) return bounce_right(g);
  if (collides(ball_rect(gg), right_side_of_the_screen)) return bounce_left(g);
  if (collides(ball_rect(gg), bottom_of_the_screen)) return bounce_up(g);
  if (collides(ball_rect(gg), top_of_the_screen)) return bounce_down(g);
  
  double paddle_dy = (g.key_up_is_pressed ? paddle_speed : 0.0)
                   + (g.key_down_is_pressed ? -paddle_speed : 0.0);
  
  gg.left_paddle_y += paddle_dy;
  
  return gg;
}

void advance_simulation() {
  game_state = next(game_state);
}

void draw_frame() {
  clear();
  
  draw(left_paddle_rect(), paddle_color);
  draw(ball_rect(), ball_color);
}
