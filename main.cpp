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


void handle_event(const SDL_Event& event) {
  if(event.key.keysym.sym == SDLK_c) {
    printf("switch_color\n");
    current_fg = rgb(0x0000ff);
  }
}

GameState next(const GameState& g) {
  GameState gg = g;
  
  gg.ball_pos = g.ball_pos + g.ball_speed;
  
  return gg;
}

void advance_simulation() {
  game_state = next(game_state);
}

void draw_frame() {
  clear();
  
  draw(rect(screen_left  + V2(0.03, game_state.left_paddle_y ), paddle_size), paddle_color);
  draw(rect(screen_right - V2(0.03, game_state.right_paddle_y), paddle_size), paddle_color);
  
  draw(square(game_state.ball_pos, ball_radius), ball_color);
}
