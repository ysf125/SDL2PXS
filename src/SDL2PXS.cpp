#include "SDL2PXS.hpp"

// private area

void SDL2PXS::setup() {
    pixels.resize(pixelsInY * pixelsInX);
    
    if (gridSize > 0) {
        surface = SDL_CreateRGBSurface(0, *width, *height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
        Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
        SDL_FillRect(surface, NULL, color);
        drawBoundariesOfGrid({ 0, 0 }, pixelsInX, pixelsInY);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    setDrawColor();
    clearTheScreen();  
}

xy<int> SDL2PXS::getStartOfPixelPos(xy<int> pointPos) {
    int x = (pointPos.x * PXSize) + (pointPos.x * gridSize), 
        y = (pointPos.y * PXSize) + (pointPos.y * gridSize);
    return { x, y };
}

void SDL2PXS::drawBoundariesOfGrid(xy<int> stratPointPos, int W, int H) {
    //SDL_Rect * rect = new SDL_Rect;
    //Uint32 color = SDL_MapRGBA(surface->format, gridColor.R, gridColor.G, gridColor.B, 255); 
}

// public area

SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int PXSize, int gridSize, RGB gridColor)
    : PXSize(PXSize),gridSize(gridSize), gridColor(gridColor), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetWindowSizeInPixels(window, width, height);
    pixelsInX = floor(*width / (PXSize + gridSize));
    pixelsInY = floor(*height / (PXSize + gridSize));
    setup();
}

SDL2PXS::SDL2PXS(char title[], int pixelsInX, int pixelsInY, int PXSize, int gridSize, RGB gridColor)
    : PXSize(PXSize), pixelsInX(pixelsInX), pixelsInY(pixelsInY), gridSize(gridSize), gridColor(gridColor) {
    SDL_Init(SDL_INIT_VIDEO);
    *width = pixelsInX * (PXSize + gridSize);
    *height = pixelsInY * (PXSize + gridSize);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    setup();
}

void SDL2PXS::closeSDL2PXS() {
    SDL_Quit();
    delete width;
    delete height;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
}

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

void SDL2PXS::showChanges() { 
    SDL_RenderPresent(renderer); 
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

void SDL2PXS::clearTheScreen() {
    SDL_RenderClear(renderer);  
    S fill(pixels.begin(), pixels.end(), drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, texture, NULL, NULL);
}

RGB SDL2PXS::getPixleColor(xy<int> pixelPos) {
    RGB color = pixels[(pixelPos.y * pixelsInX) + pixelPos.x];
    return color;
}

void SDL2PXS::drawPixel(xy<int> pixelPos) {
    xy<int> stratPosInPX = getStartOfPixelPos(pixelPos);
    SDL_Rect* rect = new SDL_Rect{stratPosInPX.x, stratPosInPX.y, PXSize, PXSize};
    SDL_RenderFillRect(renderer, rect);
    pixels[pixelPos.y * pixelsInX + pixelPos.x] = drawColor;
}

void SDL2PXS::drawFillRect(xy<int> stratPointPos, int W, int H) {
    xy<int> stratPosInPX = getStartOfPixelPos(stratPointPos);
    int widthInPX = (W * PXSize) + (W * gridSize), heightInPX = (H * PXSize) + (H * gridSize);
    SDL_Rect* rect = new SDL_Rect{stratPosInPX.x, stratPosInPX.y, widthInPX, heightInPX};
    SDL_RenderFillRect(renderer, rect);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            pixels[(stratPointPos.y + i) * pixelsInX + (stratPointPos.x + j)] = drawColor;
        }
    }    
}

//void SDL2PXS::drawRect(xy<int> stratPointPos, int W, int H) {}