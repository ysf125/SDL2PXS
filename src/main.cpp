#include "SDL2PXS.hpp"
#include <queue>
#include <random>
#define S std::

int randomInt(int min, int max);

bool isPixelOnTop(xy<int> pixel0, xy<int> pixel1);

int main(int argc, char* args[]) {

	// Setup for SDL2PXS
	SDL_Init(SDL_INIT_VIDEO);
	Uint32 flags = SDL_WINDOW_SHOWN;
	SDL_Window* window = SDL_CreateWindow("SDL2PXS snake game test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL2PXS screen = SDL2PXS(window, renderer, 15, 2, { 128, 128, 128 });

	screen.setDrawColor();
	screen.clearTheScreen();
	screen.showChanges();

	// Variables for the game
	int* pixelsInX = new int, * pixelsInY = new int;
	screen.getPixelsInXAndY(pixelsInX, pixelsInY);

	int movingDirection = 0;
	xy<int> food = { randomInt(0 , *pixelsInX - 1), randomInt(0 , *pixelsInY - 1) };
	S queue<xy<int>> snake;
	snake.push((xy<int>) { (int)floor(*pixelsInX / 2), (int)floor(*pixelsInY / 2) });

	// Game loop 
	SDL_Event e;
	bool quit = false;

	while (!quit) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT: quit = true; break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_RIGHT: movingDirection = 0; break;
			case SDLK_DOWN: movingDirection = 2; break;
			case SDLK_LEFT: movingDirection = 4; break;
			case SDLK_UP: movingDirection = 6; break;
			} break;
		}


		screen.setDrawColor({ 255, 0, 0 });
		screen.drawPixel(food);

		screen.setDrawColor({ 0, 255, 0 });
		snake.push(movePointInGrid(snake.back(), movingDirection));
		screen.drawPixel(snake.back());

		screen.setDrawColor({ 0, 0, 0 });
		screen.drawPixel(snake.front());
		snake.pop();

		screen.showChanges();
		SDL_Delay(33);
	}

	screen.closeSDL2PXS();

	return 0;
}

int randomInt(int min, int max) {
	S random_device rd;
	S mt19937 mt(rd());
	S uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}

bool isPixelOnTop(xy<int> pixel0, xy<int> pixel1) {
	if (pixel0.x == pixel1.x && pixel0.y == pixel1.y) return true;
	return false;
}