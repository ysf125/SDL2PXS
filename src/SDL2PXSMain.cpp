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
    if (gridSize > 0) {
        surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
        Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
        SDL_FillRect(surface, NULL, color);
        drawGrid();
        gridTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    PXSplane.pixels.resize(PXSplane.pixelsInX * PXSplane.pixelsInY);
    setDrawColor();
    clearTheScreen();
}

xy<int> SDL2PXS::getStartOfPixelPos(xy<int> pixel) {
    int x = (pixel.x * PXSize) + (pixel.x * gridSize),
        y = (pixel.y * PXSize) + (pixel.y * gridSize);
    return { x, y };
}

void SDL2PXS::setPixelColor(xy<int> pixel) {
    if (notInsideTheScreen(pixel)) return;
    PXSplane.pixels[(pixel.y * PXSplane.pixelsInY) + pixel.x] = drawColor;
}

void SDL2PXS::correctNegativeWH(xy<int>& startPixel, int& W, int& H) {
    if (W < 0) { startPixel.x += (W + 1); W = abs(W); }
    if (H < 0) { startPixel.y += (H + 1); H = abs(H); }
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

void SDL2PXS::getWidthAndHeight(int& W, int& H) {
    W = width;
    H = height;
}

void SDL2PXS::getPixelsInXAndY(int& pixelsInX, int& pixelsInY) {
    pixelsInX = PXSplane.pixelsInX;
    pixelsInY = PXSplane.pixelsInY;
}

RGB SDL2PXS::getDrawColor() { return drawColor; }

options SDL2PXS::getPXSOptions() { return PXSOptions; }

void SDL2PXS::closeSDL2PXS() {
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gridTexture);
}

void SDL2PXS::showChanges() { SDL_RenderPresent(renderer); }

bool SDL2PXS::notInsideTheScreen(xy<int> pixel) {
    return (pixel.x < 0 || pixel.y < 0 || pixel.x > PXSplane.pixelsInX || pixel.y > PXSplane.pixelsInY);
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

RGB SDL2PXS::getPixleColor(xy<int> pixel) {
    if (notInsideTheScreen(pixel)) return { 0, 0, 0 };
    return PXSplane.pixels[(pixel.y * PXSplane.pixelsInY) + pixel.x];
}

plane2D SDL2PXS::copyFromScreen(xy<int> startPixel, int W, int H) {
    correctNegativeWH(startPixel, W, H);
    
    plane2D returnedPlane = { W, H };
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            RGB pixelColor = getPixleColor({startPixel.x + j , startPixel.y + i});              
            returnedPlane.pixels[(startPixel.x + j) + (startPixel.y + i) * W] = pixelColor;
        }
    }
    return returnedPlane;
}

plane2D SDL2PXS::copyFromPlane(plane2D& plane, xy<int> startPixel, int W, int H) {
    correctNegativeWH(startPixel, W, H);

    plane2D returnedPlane = { W, H };
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            RGB pixelColor = plane.pixels[(startPixel.x + j) + (startPixel.y + i) * plane.pixelsInX];              
            returnedPlane.pixels[(startPixel.x + j) + (startPixel.y + i) * W] = pixelColor;
        }
    }
    return returnedPlane;
}