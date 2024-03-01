#include "SDL2PXS.hpp"

// Private area
void SDL2PXS::setup() {
    if ((PXSOptions & noOverflow) == noOverflow) {
        *width = (PXSplane.pixelsInX * PXSize) + ((PXSplane.pixelsInX - 1) * gridSize);
        *height = (PXSplane.pixelsInY * PXSize) + ((PXSplane.pixelsInY - 1) * gridSize);
        SDL_SetWindowSize(window, *width, *height);
    }
    if (gridSize > 0) {
        surface = SDL_CreateRGBSurface(0, *width, *height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
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

void SDL2PXS::drawGrid() {
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>();
    Uint32 color = SDL_MapRGB(surface->format, gridColor.R, gridColor.G, gridColor.B);

    for (int i = 0; i < PXSplane.pixelsInX; i++) {
        *rect = { (PXSize * (i + 1)) + (gridSize * i), 0, gridSize, *height };
        SDL_FillRect(surface, rect.get(), color);
    }
    for (int i = 0; i < PXSplane.pixelsInY; i++) {
        *rect = SDL_Rect{ 0, (PXSize * (i + 1)) + (gridSize * i), *width, gridSize };
        SDL_FillRect(surface, rect.get(), color);
    }
}

// Public area
SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions, int gridSize, RGB gridColor)
    : PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GetWindowSizeInPixels(window, width.get(), height.get());
    if (pixelsInX <= 0) PXSplane.pixelsInX = ceil(*width / (PXSize + gridSize));
    if (pixelsInY <= 0) PXSplane.pixelsInY = ceil(*height / (PXSize + gridSize));
    setup();
}

SDL2PXS::SDL2PXS(S string windowTitle, int W, int H, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions, int gridSize, RGB gridColor)
    : PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSplane({ pixelsInX, pixelsInY }) {
        SDL_Init(SDL_INIT_VIDEO);
        *this->width = W; *this->height = H;
        if (pixelsInX <= 0) PXSplane.pixelsInX = ceil(W / (PXSize + gridSize));
        if (pixelsInY <= 0) PXSplane.pixelsInY = ceil(H / (PXSize + gridSize));
        this->window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
        this->renderer = SDL_CreateRenderer(window, -1, 0);
        setup();
}    

void SDL2PXS::closeSDL2PXS() {
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gridTexture);
}

void SDL2PXS::getWidthAndHeight(int& W, int& H) {
    W = *this->width;
    H = *this->height;
}

void SDL2PXS::getPixelsInXAndY(int& pixelsInX, int& pixelsInY) {
    pixelsInX = this->PXSplane.pixelsInX;
    pixelsInY = this->PXSplane.pixelsInY;
}

void SDL2PXS::showChanges() { SDL_RenderPresent(renderer); }

void SDL2PXS::clearTheScreen() {
    SDL_RenderClear(renderer);
    S fill(PXSplane.pixels.begin(), PXSplane.pixels.end(), drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, gridTexture, NULL, NULL);
}

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

void SDL2PXS::drawPixel(xy<int> pixel) {
    xy<int> stratPosInPX = getStartOfPixelPos(pixel);
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(stratPosInPX.x, stratPosInPX.y, PXSize, PXSize);
    SDL_RenderFillRect(renderer, rect.get());

    setPixelColor(pixel);
}

void SDL2PXS::drawRect(xy<int> startPixel, int W, int H) {
    correctNegativeWH(startPixel, W, H);

    drawFillRect(startPixel, W, 1);
    drawFillRect(startPixel, 1, H);
    drawFillRect({ startPixel.x, startPixel.y + (H - 1) }, W, 1);
    drawFillRect({ startPixel.x + (W - 1), startPixel.y }, 1, H);
}

void SDL2PXS::drawFillRect(xy<int> startPixel, int W, int H) {
    correctNegativeWH(startPixel, W, H);

    xy<int> stratPosInPX = getStartOfPixelPos(startPixel);
    int widthInPX = (W * PXSize) + ((W - 1) * gridSize),
        heightInPX = (H * PXSize) + ((H - 1) * gridSize);
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(stratPosInPX.x, stratPosInPX.y, widthInPX, heightInPX);
    SDL_RenderFillRect(renderer, rect.get());

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            setPixelColor({ startPixel.x + j, startPixel.y + i });
        }
    }

    if (gridSize > 0) {
        *rect = SDL_Rect{ stratPosInPX.x - gridSize, stratPosInPX.y - gridSize, widthInPX + gridSize, heightInPX + gridSize };
        SDL_RenderCopy(renderer, gridTexture, rect.get(), rect.get());
    }
}

void SDL2PXS::drawLine(xy<int> pixel0, xy<int> pixel1) {
    for (xy<int> point : angline(pixel0, pixel1)) drawPixel(point);
}

void SDL2PXS::drawCircle(xy<int> centerPixel, int R) {
    R = abs(R);
    xy<int> xyTemp = { 0, R };
    S vector<xy<int>> points = { {xyTemp.x, xyTemp.y} };
    int P = 1 - R;

    while (xyTemp.x <= xyTemp.y) {
        xyTemp.x++;
        if (P < 0) { P = P + 2 * xyTemp.x + 1; }
        else {
            xyTemp.y--;
            P = P - 2 * xyTemp.y + 2 * xyTemp.x + 1;
        }
        points.push_back({ xyTemp.x, xyTemp.y });
    }

    for (xy<int> point : points) {
        drawPixel({ centerPixel.x + point.x, centerPixel.y + point.y });
        drawPixel({ centerPixel.x - point.x, centerPixel.y + point.y });
        drawPixel({ centerPixel.x + point.x, centerPixel.y - point.y });
        drawPixel({ centerPixel.x - point.x, centerPixel.y - point.y });
        drawPixel({ centerPixel.x + point.y, centerPixel.y + point.x });
        drawPixel({ centerPixel.x - point.y, centerPixel.y + point.x });
        drawPixel({ centerPixel.x + point.y, centerPixel.y - point.x });
        drawPixel({ centerPixel.x - point.y, centerPixel.y - point.x });
    }
}

void SDL2PXS::drawFillCircle(xy<int> centerPixel, int R) {
    drawCircle(centerPixel, R);
    fill(centerPixel, getPixleColor(centerPixel));
}

void SDL2PXS::fill(xy<int> startPixel, RGB oldColor) {
    S stack<xy<int>> pixelsToProcess;
    pixelsToProcess.push(startPixel);

    while (pixelsToProcess.size() != 0) {
        xy<int> pixel = pixelsToProcess.top();
        RGB pixelColor = getPixleColor(pixel);

        if (!(pixelColor.R == oldColor.R && pixelColor.G == oldColor.G && pixelColor.B == oldColor.B) || (notInsideTheScreen(pixel))) {
            pixelsToProcess.pop();
            continue;
        }

        drawPixel(pixel);
        pixelsToProcess.pop();
        for (int i = 0; i < 4; i++) pixelsToProcess.push(movePointInGrid(pixel, i * 2));
    }
}