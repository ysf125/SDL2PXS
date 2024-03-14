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
    int x = (pixel.x * PXSize) + (pixel.x * gridSize),
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

void SDL2PXS::drawGrid() {
    SDL_DestroyTexture(gridTexture);
    surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>();
    Uint32 color = SDL_MapRGB(surface->format, gridColor.R, gridColor.G, gridColor.B);
    for (int i = 0; i < PXSplane.pixelsInX; i++) {
        *rect = { getStartOfPixelPos({ i, 0 }).x - (gridSize + startPixel.x), 0, gridSize, height };
        SDL_FillRect(surface, rect.get(), color);
    }
    for (int i = 0; i < PXSplane.pixelsInY; i++) {
        *rect = SDL_Rect{ 0, getStartOfPixelPos({ 0, i }).y - (gridSize + startPixel.y), width, gridSize };
        SDL_FillRect(surface, rect.get(), color);
    }

    gridTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Public area

// Main section

SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
options PXSOptions, xy<int> startPixel, int gridSize, RGB gridColor)
    : startPixel(startPixel), width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    setup();
}

SDL2PXS::SDL2PXS(S string windowTitle, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
options PXSOptions, xy<int> startPixel, int gridSize, RGB gridColor)
    : startPixel(startPixel), width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    setup();
}

void SDL2PXS::closeSDL2PXS() {
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gridTexture);
}

void SDL2PXS::showChanges() { SDL_RenderPresent(renderer); }


void SDL2PXS::clearTheScreen() {
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(startPixel.x, startPixel.y, width, height);
    SDL_RenderFillRect(renderer, rect.get());
    for (int i = 0; i < PXSplane.pixelsInY; i++) S fill(PXSplane.pixels[i].begin(), PXSplane.pixels[i].end(), drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, gridTexture, NULL, rect.get());
}