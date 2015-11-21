#include <math.h>
#include "engine.cpp"


double ball_radius = 0.02;
V2 paddle_size = V2(0.03, 0.2);

Color ball_color;
Color paddle_color;

void init() {
  ball_color = rgb(0xccffcc);
  paddle_color = rgb(0x88aa88);
}


struct GameState {
  double left_paddle_y = 0.0;
  double right_paddle_y = 0.0;
  
  V2 ball_pos = screen_center;
  V2 ball_speed = V2(0.01, 0.01);
} game_state;

inline Rect ball_rect(const GameState& g=game_state) {
  return square_at(ball_radius*2, g.ball_pos);
}

inline Rect left_paddle_rect(const GameState& g=game_state) {
  return rect_at(paddle_size, screen_left + V2(0.03, g.left_paddle_y));
}

inline Rect right_paddle_rect(const GameState& g=game_state) {
  return rect_at(paddle_size, screen_right - V2(0.03, g.right_paddle_y));
}


void handle_event(const SDL_Event& event) {
  if(event.key.keysym.sym == SDLK_c) {
    printf("switch_color\n");
    current_fg = rgb(0x0000ff);
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
  if (collides(ball_rect(gg), right_paddle_rect(gg))) return bounce_left(g);
  if (gg.ball_pos.y < screen_bottom.y) return bounce_up(g);
  if (gg.ball_pos.y > screen_top.y) return bounce_down(g);
  
  return gg;
}

void advance_simulation() {
  game_state = next(game_state);
}

void draw_frame() {
  clear();
  
  draw(left_paddle_rect(), paddle_color);
  draw(right_paddle_rect(), paddle_color);
  draw(ball_rect(), ball_color);
}
