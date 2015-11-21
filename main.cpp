#include "engine.cpp"


struct GameState {
} game_state;


void handle_event(const SDL_Event& event) {
}

GameState next(const GameState& g) {
  GameState gg = g;
  
  return gg;
}

void advance_simulation() {
  game_state = next(game_state);
}

void draw_frame() {
}
