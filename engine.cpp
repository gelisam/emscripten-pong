#include <assert.h>
#include <emscripten.h>
#include <SDL/SDL.h>

SDL_Surface *screen;
const int WIDTH = 800;
const int HEIGHT = 600;


double infinity = 1.0 / 0.0;
double screen_ratio = 1.0 * WIDTH / HEIGHT;


struct V2 {
  double x, y;
  V2(double x, double y)
  : x(x), y(y) {}
};

V2 screen_top    = V2(          0.0, 1.0);
V2 screen_left   = V2(-screen_ratio, 0.0);
V2 screen_center = V2(          0.0, 0.0);
V2 screen_right  = V2( screen_ratio, 0.0);
V2 screen_bottom = V2(          0.0,-1.0);

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


struct Rect {
  double n, e, w, s;
  Rect(double n, double e, double w, double s)
  : n(n), e(e), w(w), s(s) {}
};

Rect screen_rect = Rect(1, 1, -1, -1);
Rect top_of_the_screen        = Rect(infinity, infinity, -infinity, 1);
Rect right_side_of_the_screen = Rect(infinity, infinity, screen_ratio, -infinity);
Rect left_side_of_the_screen  = Rect(infinity, -screen_ratio, -infinity, -infinity);
Rect bottom_of_the_screen     = Rect(-1, infinity, -infinity, -infinity);

inline bool collides(const Rect& a, const Rect& b) {
  return (a.n >= b.s && a.e >= b.w && a.w <= b.e && a.s <= b.n);
}

inline bool contains(const Rect& a, const Rect& b) {
  return (a.n >= b.n && a.e >= b.e && a.w <= b.w && a.s <= b.s);
}

inline SDL_Rect sdl_rect(const Rect& r) {
  V2 pixel_pos = to_pixel_coords(V2(r.w, r.n));
  V2 pixel_dim = to_pixel_magnitude(V2(r.e-r.w, r.n-r.s));
  
  SDL_Rect rect;
  rect.x = pixel_pos.x;
  rect.y = pixel_pos.y;
  rect.w = pixel_dim.x;
  rect.h = pixel_dim.y;
  return rect;
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


inline Rect rect_at(const V2& dim, const V2& pos) {
  return Rect(pos.y + dim.y / 2, pos.x + dim.x / 2, pos.x - dim.x / 2, pos.y - dim.y /2);
}

inline Rect square_at(double side, const V2& pos) {
  return rect_at(V2(side, side), pos);
}


inline void clear(Color color=current_bg) {
  assert(SDL_FillRect(screen, NULL, color) == 0);
}

void draw(const SDL_Rect& r, Color color=current_fg) {
  assert(SDL_FillRect(screen, &r, color) == 0);
}

void draw(const Rect& r, Color color=current_fg) {
  draw(sdl_rect(r), color);
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
