#include "SDL2PXS.hpp"

int main( int argc, char* args[] ) {
	
  int const W = 800;
  int const H = 600;

	SDL_Init(SDL_INIT_VIDEO);
  
  SDL_Window * window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_RESIZABLE);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Surface * surface =  SDL_GetWindowSurface(window);
  SDL_Texture * texture =  SDL_CreateTextureFromSurface(renderer,surface);
  
  SDL_Event e; 
  bool quit = false;
  SDL_Rect * rect0 = new SDL_Rect {0,0,150,150};
  SDL_Rect * rect1 = new SDL_Rect {50,50,150,150};      

  while (!quit) {
    SDL_PollEvent(&e);
    switch (e.type) {
      case SDL_QUIT : quit = true; break; 
    }  
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
   
    SDL_RenderCopy(renderer,texture,rect0,rect1);

    SDL_RenderPresent(renderer);
    SDL_Delay(16);
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderClear(renderer);
  } 

  SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}