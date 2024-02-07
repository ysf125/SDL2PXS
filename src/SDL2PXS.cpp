#include "SDL2PXS.hpp"

// private area

void SDL2PXS::setup() {
    if (gridSize > 0) {
        surface = SDL_CreateRGBSurface(0, *width, *height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
        Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
        SDL_FillRect(surface, NULL, color);
        drawGrid();
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    pixels.resize(pixelsInY * pixelsInX);
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
    pixels[pixel.y * pixelsInX + pixel.x] = drawColor;
}

void SDL2PXS::drawGrid() {
    SDL_Rect* rect = new SDL_Rect;
    Uint32 color = SDL_MapRGBA(surface->format, gridColor.R, gridColor.G, gridColor.B, 255);

    for (int i = 0; i < pixelsInX; i++) {
        rect = new SDL_Rect{ (PXSize * (i + 1)) + (gridSize * i), 0, gridSize, *height };
        SDL_FillRect(surface, rect, color);
    }
    for (int i = 0; i < pixelsInY; i++) {
        rect = new SDL_Rect{ 0, (PXSize * (i + 1)) + (gridSize * i), *width, gridSize };
        SDL_FillRect(surface, rect, color);
    }
    delete rect;
}

// public area

SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int PXSize, int gridSize, RGB gridColor)
    : PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), window(window), renderer(renderer) {
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

void SDL2PXS::clearTheScreen() {
    SDL_RenderClear(renderer);
    S fill(pixels.begin(), pixels.end(), drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, texture, NULL, NULL);
}

bool SDL2PXS::notInsideTheScreen(xy<int> pixel) {
    return (pixel.x < 0 || pixel.y < 0 || pixel.x >= pixelsInX || pixel.y >= pixelsInX); 
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

RGB SDL2PXS::getPixleColor(xy<int> pixel) {
    if (notInsideTheScreen(pixel)) return { 0, 0, 0 };
    RGB color = pixels[(pixel.y * pixelsInX) + pixel.x];
    return color;
}

void SDL2PXS::drawPixel(xy<int> pixel) {
    xy<int> stratPosInPX = getStartOfPixelPos(pixel);
    SDL_Rect* rect = new SDL_Rect{ stratPosInPX.x, stratPosInPX.y, PXSize, PXSize };
    SDL_RenderFillRect(renderer, rect);

    setPixelColor(pixel);
}

void SDL2PXS::drawFillRect(xy<int> startPixel, int W, int H) {
    xy<int> stratPosInPX = getStartOfPixelPos(startPixel);
    int widthInPX = (W * PXSize) + ((W - 1) * gridSize),
        heightInPX = (H * PXSize) + ((H - 1) * gridSize);
    SDL_Rect* rect = new SDL_Rect{ stratPosInPX.x, stratPosInPX.y, widthInPX, heightInPX };
    SDL_RenderFillRect(renderer, rect);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            setPixelColor({ startPixel.x + j, startPixel.y + i });
        }
    }

    if (gridSize > 0) {
        int halfGridSize = (gridSize / 2);
        rect = new SDL_Rect{ stratPosInPX.x - halfGridSize, stratPosInPX.y - halfGridSize, widthInPX + halfGridSize, heightInPX + halfGridSize };
        SDL_RenderCopy(renderer, texture, rect, rect);
    }
}

// First try !!!
void SDL2PXS::drawRect(xy<int> startPixel, int W, int H) {
    drawFillRect(startPixel, W, 1);
    drawFillRect(startPixel, 1, H);
    drawFillRect({ startPixel.x, startPixel.y + (H - 1) }, W, 1);
    drawFillRect({ startPixel.x + (W - 1), startPixel.y }, 1, H);
}

void SDL2PXS::drawLine(xy<int> pixel0, xy<int> pixel1) {
    for (xy<int> point : angline(pixel0, pixel1)) drawPixel(point);
}

//void drawCircle(xy<int> centerPixel, int R) {}

void SDL2PXS::floodFill(xy<int> startPixel, RGB oldColor) {
    if (notInsideTheScreen(startPixel)) return; 
    RGB pointColor = getPixleColor(startPixel);
    if (pointColor.R == oldColor.R && pointColor.G == oldColor.G && pointColor.B == oldColor.B) {
        drawPixel(startPixel);
        for (int i = 0; i < 4; i++) {
            floodFill(movePointInGrid(startPixel,i), oldColor);
        }
    }
}
