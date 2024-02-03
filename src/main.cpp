#include "SDL2PXS.hpp"

int main(int argc, char* args[]) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL2PXS screen = SDL2PXS(5, window, renderer);

	screen.setDrawColor({ 0, 0, 255 });
	screen.clearTheScreen();
	screen.showChanges();

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT: quit = true; break;
		}
	}

	return 0;
}