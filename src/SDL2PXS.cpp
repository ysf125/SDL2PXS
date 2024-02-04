#include "SDL2PXS.hpp"

// private area

void SDL2PXS::setup() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    setDrawColor();  
    
    pixels.resize((*width) * (*height));
    S fill(pixels.begin(), pixels.end(), drawColor);

    surface = SDL_GetWindowSurface(window);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

xy<int> SDL2PXS::getStartOfPixelPos(xy<int> pointPos) {
    int x = (pointPos.x * PXSize) + (pointPos.x * gridSize), 
        y = (pointPos.y * PXSize) + (pointPos.y * gridSize);
    return { x, y };
}

//void SDL2PXS::drawBoundariesOfGrid(xy<int> stratPointPos = { 0, 0 }, int W, int H) {}

// public area

SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int PXSize)
    : PXSize(PXSize), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetWindowSizeInPixels(window, width, height);
    pixelsInX = floor(*width / (PXSize + gridSize));
    pixelsInY = floor(*height / (PXSize + gridSize));
    setup();
}

SDL2PXS::SDL2PXS(char title[], int PXSize, int pixelsInX, int pixelsInY)
    : PXSize(PXSize), pixelsInX(pixelsInX), pixelsInY(pixelsInY) {
    SDL_Init(SDL_INIT_VIDEO);
    *width = pixelsInX * (PXSize + gridSize);
    *height = pixelsInY * (PXSize + gridSize);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, -0);
    setup();
}

SDL2PXS::~SDL2PXS() {
    SDL_Quit();
    delete width;
    delete height;
    delete surface;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
}

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

void SDL2PXS::showChanges() { SDL_RenderPresent(renderer); }

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

void SDL2PXS::clearTheScreen() {
    SDL_RenderClear(renderer);  
    S fill(pixels.begin(), pixels.end(), drawColor);
}

RGB SDL2PXS::getPixleColor(xy<int> pixelPos) {
    RGB color;
    color = pixels[(pixelPos.y * (*width)) + pixelPos.x];
    return color;
}

void SDL2PXS::drawPixel(xy<int> pixelPos) {
    xy<int> stratPosInPX = getStartOfPixelPos(pixelPos);
    SDL_Rect* rect = new SDL_Rect{stratPosInPX.x, stratPosInPX.y, PXSize, PXSize};
    SDL_RenderFillRect(renderer, rect);
    pixels[(pixelPos.y * *width) + pixelPos.x] = drawColor;
}

void SDL2PXS::drawFillRect(xy<int> stratPointPos, int W, int H) {
    xy<int> stratPosInPX = getStartOfPixelPos(stratPointPos);
    int widthInPX = (W * PXSize) + (W * gridSize), heightInPX = (H * PXSize) + (H * gridSize);
    SDL_Rect* rect = new SDL_Rect{stratPosInPX.x, stratPosInPX.y, widthInPX, heightInPX};
    SDL_RenderFillRect(renderer, rect);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            pixels[((stratPointPos.y + i) * *width) + (stratPointPos.x + j)] = drawColor;
        }
    }    
}

//void SDL2PXS::drawRect(xy<int> stratPointPos, int W, int H) {}