#include "SDL2PXS.hpp"

// Private area
void SDL2PXS::setup() {
    if ((PXSOptions & autoWidthAndHeight) == autoWidthAndHeight) {
        width = (PXSplane.pixelsInX * PXSize) + ((PXSplane.pixelsInX - 1) * gridSize);
        height = (PXSplane.pixelsInY * PXSize) + ((PXSplane.pixelsInY - 1) * gridSize);
    }

    if ((PXSOptions & autoPixelsInXAndY) == autoPixelsInXAndY) {
        PXSplane.pixelsInX = ceil(width / (PXSize + gridSize));
        PXSplane.pixelsInY = ceil(height / (PXSize + gridSize));
    }

    if ((PXSOptions & resizeTheScreen) == resizeTheScreen) SDL_SetWindowSize(window, width, height);

    if (gridSize > 0) drawGrid(); 

    PXSplane.pixels.resize(PXSplane.pixelsInY);
    for (int i = 0; i < PXSplane.pixelsInY; i++) PXSplane.pixels[i].resize(PXSplane.pixelsInX);
    setDrawColor();
    clearTheScreen();
}

xy<int> SDL2PXS::getStartOfPixelPos(xy<int> pixel) {
    int x = (pixel.x * PXSize) + (pixel.x * gridSize) ,
        y = (pixel.y * PXSize) + (pixel.y * gridSize);
    return { x + startPixel.x, y + startPixel.y };
}

void SDL2PXS::setPixelColor(xy<int> pixel) {
    if (notInsideTheScreen(pixel)) return;
    PXSplane.pixels[pixel.y][pixel.x] = drawColor;
}

void SDL2PXS::correctNegativeWidthAndHeight(SDL_Rect& rect) {
    if (rect.w < 0) { rect.x += (rect.w + 1); rect.w = abs(rect.w); }
    if (rect.h < 0) { rect.y += (rect.h + 1); rect.h = abs(rect.h); }
}

// Public area
SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int W, int H, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions, int gridSize, RGB gridColor)
    : width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    setup();
}

SDL2PXS::SDL2PXS(S string windowTitle, int W, int H, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions, int gridSize, RGB gridColor)
    : width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    setup();
}

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

S tuple<int, int> SDL2PXS::getWidthAndHeight() { return { width, height }; }

S tuple<int, int> SDL2PXS::getPixelsInXAndY() { return { PXSplane.pixelsInX, PXSplane.pixelsInY }; }

RGB SDL2PXS::getDrawColor() { return drawColor; }

options SDL2PXS::getPXSOptions() { return PXSOptions; }

void SDL2PXS::closeSDL2PXS() {
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gridTexture);
}

void SDL2PXS::showChanges() { SDL_RenderPresent(renderer); }

bool SDL2PXS::notInsideTheScreen(xy<int> pixel) { return notInsideThePlane(PXSplane, pixel); }

bool SDL2PXS::notInsideThePlane(plane2D& plane, xy<int> pixel) {
    return (pixel.x < 0 || pixel.y < 0 || pixel.x > plane.pixelsInX || pixel.y > plane.pixelsInY);
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

RGB SDL2PXS::getPixleColor(xy<int> pixel) { return getPixleColorFromPlane(PXSplane, pixel); }

RGB SDL2PXS::getPixleColorFromPlane(plane2D& plane, xy<int> pixel) {
    if (notInsideThePlane(plane, pixel)) return { 0, 0, 0 };
    return plane.pixels[pixel.y][pixel.x];
}

plane2D SDL2PXS::copyFromScreen(SDL_Rect src) { return copyFromPlane(PXSplane, src); }

plane2D SDL2PXS::copyFromPlane(plane2D& plane, SDL_Rect src) {
    correctNegativeWidthAndHeight(src);

    RGB color;
    plane2D returnedPlane = { src.w, src.h };
    returnedPlane.pixels.resize(src.h);
    for (int i = 0; i < src.h; i++) {
        returnedPlane.pixels[i].resize(src.w);
        for (int j = 0; j < src.w; j++) {
            color = getPixleColorFromPlane(plane, { src.x + j, src.y + i });
            returnedPlane.pixels[i][j] = color;
        }
    }
    return returnedPlane;
}