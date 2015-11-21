#include <assert.h>
#include <emscripten.h>
#include <SDL/SDL.h>

SDL_Surface *screen;
const int WIDTH = 800;
const int HEIGHT = 600;


struct V2 {
  double x, y;
  V2(double x, double y)
  : x(x), y(y) {}
};

V2 screen_top    = V2( 0.0, 1.0);
V2 screen_left   = V2(-1.0, 0.0);
V2 screen_center = V2( 0.0, 0.0);
V2 screen_right  = V2( 1.0, 0.0);
V2 screen_bottom = V2( 0.0,-1.0);

V2 pixels_screen_center = V2(WIDTH / 2.0, HEIGHT / 2.0);
double pixels_per_unit = HEIGHT / 2.0;

inline V2 operator+(const V2& a, const V2& b) {
  return V2(a.x + b.x, a.y + b.y);
}

inline V2 operator-(const V2& a, const V2& b) {
  return V2(a.x - b.x, a.y - b.y);
}

inline V2 operator*(const V2& a, const V2& b) {
  return V2(a.x * b.x, a.y * b.y);
}

inline V2 operator*(double a, const V2& b) {
  return V2(a * b.x, a * b.y);
}

inline V2 operator/(const V2& a, double b) {
  return V2(a.x / b, a.y / b);
}

inline double to_pixel_magnitude(double a) {
  return pixels_per_unit * a;
}

inline V2 to_pixel_magnitude(const V2& a) {
  return pixels_per_unit * a;
}

inline V2 to_pixel_coords(const V2& a) {
  return V2(1, -1) * to_pixel_magnitude(a) + pixels_screen_center;
}



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

Color current_fg;
Color current_bg;


double t_at_start, t_at_last_frame, t, dt;


inline SDL_Rect rect(const V2& pos, const V2& dim) {
  V2 pixel_pos = to_pixel_coords(pos);
  V2 pixel_dim = to_pixel_magnitude(dim);
  
  SDL_Rect rect;
  rect.x = pixel_pos.x;
  rect.y = pixel_pos.y;
  rect.w = pixel_dim.x;
  rect.h = pixel_dim.y;
  return rect;
}

inline SDL_Rect square(const V2& pos, double radius=0.01) {
  return rect(pos - V2(radius, radius), 2*V2(radius, radius));
}


inline void clear(Color color=current_bg) {
  assert(SDL_FillRect(screen, NULL, color) == 0);
}

void draw(const SDL_Rect& r, Color color=current_fg) {
  assert(SDL_FillRect(screen, &r, color) == 0);
}


void init();
void handle_event(const SDL_Event& event);
void advance_simulation();
void draw_frame();

void main_loop() {
  t = emscripten_get_now() - t_at_start;
  dt = t - t_at_last_frame;
  t_at_last_frame = t;
  
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    handle_event(event);
  }
  
  advance_simulation();
  
  draw_frame();
  SDL_Flip(screen);
}
 
int main() {
  assert(SDL_Init(SDL_INIT_VIDEO) >= 0);
  
  screen=SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_ANYFORMAT);
  assert(screen);
  
  t_at_start = emscripten_get_now();
  
  current_fg = rgb(0xffffff);
  current_bg = rgb(0x000000);
  init();
  
  emscripten_set_main_loop(main_loop, 0, 0);
  
  return 0;
}
