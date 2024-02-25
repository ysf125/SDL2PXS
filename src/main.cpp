#include "SDL2PXS.hpp"
#include <iostream>
#include <vector>
#include <random>
#define S std::

int randomInt(int min, int max);

bool isPixelOnTop(xy<int> pixel0, xy<int> pixel1);

int main(int argc, char* args[]) {

	// Setup for SDL2PXS
	SDL_Init(SDL_INIT_VIDEO);
	Uint32 flags = SDL_WINDOW_SHOWN;
	options PXSflags = noOverflow;
	SDL_Window* window = SDL_CreateWindow("SDL2PXS snake game test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL2PXS screen = SDL2PXS(window, renderer, 60, 45, 15, PXSflags);
	
	screen.setDrawColor();
	screen.clearTheScreen();
	screen.showChanges();

	// Variables for the game
	int* pixelsInX = new int, * pixelsInY = new int;
	screen.getPixelsInXAndY(pixelsInX, pixelsInY);
	int loop = 0;
	int movingDirection = 0;
	xy<int> food = { randomInt(0 , *pixelsInX - 2), randomInt(0 , *pixelsInY - 2) };
	S vector<xy<int>> snake = { { (int)floor(*pixelsInX / 2), (int)floor(*pixelsInY / 2) } };

	// Game loop 
	SDL_Event e;
	bool quit = false;

	while (!quit) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT: quit = true ; break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_RIGHT: movingDirection = movingDirection == 4 ? 4 : 0; break;
			case SDLK_DOWN: movingDirection = movingDirection == 6 ? 6 : 2; break;
			case SDLK_LEFT: movingDirection = movingDirection == 0 ? 0 : 4; break;
			case SDLK_UP: movingDirection = movingDirection == 2 ? 2 : 6; break;
			} break;
		}

		// Makes the game more esay and fun
		SDL_Delay(16);
		if (loop == 4) loop = 0; else { loop++; continue; }

		// Drawing everything to the screen
		screen.setDrawColor({ 255, 0, 0 });
		screen.drawPixel(food);

		screen.setDrawColor({ 0, 255, 0 });
		snake.push_back(movePointInGrid(snake[snake.size() - 1], movingDirection));
		screen.drawPixel(snake[snake.size() - 1]);

		screen.setDrawColor({ 0, 0, 0 });
		screen.drawPixel(snake[0]);
		snake.erase(snake.begin());

		screen.showChanges();

		// Logic for the game
		if (isPixelOnTop(snake[snake.size() - 1], food)) {
			snake.insert(snake.begin(), { -1, -1 });
			while (true) {
				food = { randomInt(0 , *pixelsInX - 1), randomInt(0 , *pixelsInY - 1) };
				if (screen.getPixleColor(food).G != 255) break;
			}
		}

		if (screen.notInsideTheScreen(snake[snake.size() - 1])) quit = true;

		for (uint64_t i = 0; i < snake.size() - 1; i++) {
			if (isPixelOnTop(snake[i], snake[snake.size() - 1])) quit = true;
		}
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