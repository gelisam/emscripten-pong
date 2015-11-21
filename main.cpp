#include <string.h>
#include <SDL/SDL.h>
#include <iostream>
#include <emscripten.h>
 
using namespace std;
  
SDL_Surface *screen;
 
void renderloop() {
  SDL_Flip(screen);
  SDL_LockSurface(screen);
  for(int n=0;n<1000;n++) {
    int x=rand()%800;
    int y=rand()%600;
    int pixel=rand()*100000;
    int bpp = screen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
    if((x>screen->w)||(y>screen->h)||(x<0)||(y<0)) return;
    *(Uint32 *)p = pixel;
  }
  
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if(event.key.keysym.sym == SDLK_c     ) { SDL_FillRect(screen,NULL,0); }
  }
  
  SDL_UnlockSurface(screen);
  //SDL_Quit();
}
 
int main() {
 
  
  if(SDL_Init(SDL_INIT_VIDEO)<0) {
    cout << "Failed SDL_Init " << SDL_GetError() << endl;
    return 1;
  }
  
  screen=SDL_SetVideoMode(800,600,32,SDL_ANYFORMAT);
  if(screen==NULL) {
    cout << "Failed SDL_SetVideoMode: " << SDL_GetError() << endl;
    SDL_Quit();
    return 1;
  }
  
  emscripten_set_main_loop(renderloop,0,0);
  
  return 0;
}
