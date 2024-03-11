#include "SDL2PXS.hpp"
#include <iostream>
#include <vector>
#include <random>
#define S std::

int main(int argc, char* args[]) {

	// Setup for SDL2PXS
	SDL_Init(SDL_INIT_VIDEO);
	Uint32 flags = SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow("SDL2PXS test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1018, 763, flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL2PXS screen = SDL2PXS(window, renderer, 0, 0, 60, 45, 15, (options)(autoWidthAndHeight | resizeTheScreen), 2, { 128, 128, 128 });

	SDL_Event e;
	bool quit = false;

	while (!quit) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT: quit = true; break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_LSHIFT:
				S cout << "<-- start -->\n";
				break;
			} break;
		}

		SDL_Delay(16);
		screen.showChanges();
	}

	screen.closeSDL2PXS();
	
	return 0;
}