#include "SDL2PXS.hpp"

// private area

void SDL2PXS::setup() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    surface = SDL_GetWindowSurface(window);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

xy<float> SDL2PXS::getStartOfPixelPos(xy<int> pointPos) {
    float x = pointPos.x * PXSize, y = pointPos.y * PXSize;
    return {x,y};
}

// public area

SDL2PXS::SDL2PXS(int PXSize, SDL_Window* window, SDL_Renderer* renderer)
    : PXSize(PXSize), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetWindowSizeInPixels(window, width, height);
    pixelsInX = floor(*width / PXSize);
    pixelsInY = floor(*height / PXSize);
    setup();
}

SDL2PXS::SDL2PXS(int PXSize, int pixelsInX, int pixelsInY)
    : PXSize(PXSize), pixelsInX(pixelsInX), pixelsInY(pixelsInY) {
    SDL_Init(SDL_INIT_VIDEO);
    width = new int(pixelsInX * PXSize);
    height = new int(pixelsInY * PXSize);
    window = SDL_CreateWindow("SDLPXS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, -0);
    setup();
}

SDL2PXS::~SDL2PXS() {
    delete width;
    delete height;
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

void SDL2PXS::showChanges() {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

void SDL2PXS::clearTheScreen() {
    Uint32 color = SDL_MapRGBA(surface->format, drawColor.R, drawColor.G, drawColor.B, 255);
    SDL_FillRect(surface, NULL, color);
}

RGB SDL2PXS::getPixleColor(xy<int> pixelPos) {
    RGB color = { 0, 0, 0 };
    Uint32 tempColor[3] = { 0, 0, 0 };
    Uint32* pixle = (Uint32*)surface->pixels;
    int color32bit = pixle[pixelPos.x + (pixelPos.y * surface->w)];
    tempColor[0] = color32bit >> 16;
    tempColor[1] = color32bit << 16;
    tempColor[1] = tempColor[1] >> 24;
    tempColor[2] = color32bit << 24;
    tempColor[2] = tempColor[2] >> 24;
    color.R = tempColor[0];
    color.G = tempColor[1];
    color.B = tempColor[2];
    return color;
}
