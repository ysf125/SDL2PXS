#include "SDL2PXS.hpp"

int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_VIDEO);

	Uint32 flags = SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow("0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL2PXS screen = SDL2PXS(window, renderer, 15, 2);

	screen.setDrawColor({ 128, 128, 0 });
	screen.clearTheScreen();

	screen.setDrawColor({255, 0, 0});

	

	screen.showChanges();

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT: quit = true; break;
		}

		SDL_Delay(16);
	}

	screen.closeSDL2PXS();

	return 0;
}