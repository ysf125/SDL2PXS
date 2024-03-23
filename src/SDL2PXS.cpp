#include "SDL2PXS.hpp"

void correctNegativeWidthAndHeight(SDL_Rect& rect) {
    if (rect.w < 0) { rect.x += (rect.w + 1); rect.w = abs(rect.w); }
    if (rect.h < 0) { rect.y += (rect.h + 1); rect.h = abs(rect.h); }
}

// Private 

void SDL2PXS::setup() {
    auto [pixelsInX, pixelsInY] = PXSPlane.getPixelsInXAndY();

    if ((PXSOptions & autoWidthAndHeight) == autoWidthAndHeight) {
        width = (pixelsInX * PXSize) + ((pixelsInX - 1) * gridSize);
        height = (pixelsInY * PXSize) + ((pixelsInY - 1) * gridSize);
    }

    if ((PXSOptions & autoPixelsInXAndY) == autoPixelsInXAndY) {
        PXSPlane.pixelsInX = ceil(width / (PXSize + gridSize));
        PXSPlane.pixelsInY = ceil(height / (PXSize + gridSize));
    }

    if ((PXSOptions & resizeTheScreen) == resizeTheScreen) SDL_SetWindowSize(window, width, height);
    
    if (gridSize > 0) drawGrid();

    setDrawColor();
    clearTheScreen();
}

xy<int> SDL2PXS::getStartOfPixelPos(xy<int> pixel) {
    int x = (pixel.x * PXSize) + (pixel.x * gridSize),
        y = (pixel.y * PXSize) + (pixel.y * gridSize);
    return { x + startPixel.x, y + startPixel.y };
}

void SDL2PXS::drawGrid() {
    SDL_DestroyTexture(gridTexture);
    surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>();
    Uint32 color = SDL_MapRGB(surface->format, gridColor.R, gridColor.G, gridColor.B);
    for (int i = 0; i < get<0>(PXSPlane.getPixelsInXAndY()); i++) {
        *rect = { getStartOfPixelPos({ i, 0 }).x - (gridSize + startPixel.x), 0, gridSize, height };
        SDL_FillRect(surface, rect.get(), color);
    }
    for (int i = 0; i < get<1>(PXSPlane.getPixelsInXAndY()); i++) {
        *rect = SDL_Rect{ 0, getStartOfPixelPos({ 0, i }).y - (gridSize + startPixel.y), width, gridSize };
        SDL_FillRect(surface, rect.get(), color);
    }

    gridTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Public
// Main section

SDL2PXS::SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
options PXSOptions, xy<int> startPixel, int gridSize, RGB gridColor)
    : startPixel(startPixel), width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSPlane(pixelsInX, pixelsInY), window(window), renderer(renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    setup();
}

SDL2PXS::SDL2PXS(S string windowTitle, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
options PXSOptions, xy<int> startPixel, int gridSize, RGB gridColor)
    : startPixel(startPixel), width(W), height(H), PXSize(PXSize), gridSize(gridSize), gridColor(gridColor), PXSOptions(PXSOptions), PXSPlane(pixelsInX, pixelsInY) {
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
    PXSPlane.clearThePlane(drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, gridTexture, NULL, rect.get());
}

void SDL2PXS::restart() {
    plane2D plane = PXSPlane;
    setup();
    for (int i = 0; i < get<1>(PXSPlane.getPixelsInXAndY()); i++) {
        for (int j = 0; j < get<0>(PXSPlane.getPixelsInXAndY()); j++) {
            setDrawColor(getPixleColor({ j, i }));
            drawPixel({ j, i });
        }
    }
}

bool SDL2PXS::notInsideTheScreen(xy<int> pixel) { return PXSPlane.notInsideThePlane(pixel); }

// Setters and getters section

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

S tuple<int, int> SDL2PXS::getWidthAndHeight() { return { width, height }; }

S tuple<int, int> SDL2PXS::getPixelsInXAndY() { return PXSPlane.getPixelsInXAndY(); }

RGB SDL2PXS::getDrawColor() { return drawColor; }

options SDL2PXS::getPXSOptions() { return PXSOptions; }

RGB SDL2PXS::getPixleColor(xy<int> pixel) { return PXSPlane.getPixleColor(pixel); }

void SDL2PXS::setPXSOptions(options PXSOptions) {
    this->PXSOptions = PXSOptions;
    restart();
}

void SDL2PXS::setGrid(int gridSize, RGB gridColor) {
    this->gridSize = gridSize;
    this->gridColor = gridColor;
    restart();
}

void SDL2PXS::setStartPixel(xy<int> startPixel) {
    this->startPixel = startPixel;
    restart(); 
}

void SDL2PXS::setDrawColor(RGB color) {
    drawColor = color;
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, 255);
}

// Copy and paste section

plane2D SDL2PXS::copyFromScreen(SDL_Rect src) { return PXSPlane.copyFromPlane(src); }

void SDL2PXS::pasteToScreen(plane2D& plane, SDL_Rect src, xy<int> dstStartPixel) {
    correctNegativeWidthAndHeight(src);

    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            setDrawColor(plane.getPixleColor({ src.x + j, src.y + i }));
            drawPixel({ dstStartPixel.x + j, dstStartPixel.y + i });
        }
    }
}

// Draw section

void SDL2PXS::drawPixel(xy<int> pixel) {
    xy<int> startPosInRealPixels = getStartOfPixelPos(pixel);
    SDL_RenderFillRect(renderer, S make_unique<SDL_Rect>(startPosInRealPixels.x, startPosInRealPixels.y, PXSize, PXSize).get());

    PXSPlane.drawPixel(pixel);
}

void SDL2PXS::drawRect(SDL_Rect dst) {
    correctNegativeWidthAndHeight(dst);

    drawFillRect({ dst.x, dst.y, dst.w, 1 });
    drawFillRect({ dst.x, dst.y, 1, dst.h });
    drawFillRect({ dst.x, dst.y + (dst.h - 1) , dst.w, 1 });
    drawFillRect({ dst.x + (dst.w - 1), dst.y , 1, dst.h });
}

void SDL2PXS::drawFillRect(SDL_Rect dst) {
    correctNegativeWidthAndHeight(dst);

    xy<int> startPosInRealPixels = getStartOfPixelPos({ dst.x, dst.y });
    int widthInRealPixels = (dst.w * PXSize) + ((dst.w - 1) * gridSize),
        heightInRealPixels = (dst.h * PXSize) + ((dst.h - 1) * gridSize);
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(startPosInRealPixels.x, startPosInRealPixels.y, widthInRealPixels, heightInRealPixels);
    SDL_RenderFillRect(renderer, rect.get());

    PXSPlane.drawFillRect(dst, drawColor);

    if (gridSize > 0) {
        *rect = SDL_Rect{ startPosInRealPixels.x - gridSize, startPosInRealPixels.y - gridSize, widthInRealPixels + gridSize, heightInRealPixels + gridSize };
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