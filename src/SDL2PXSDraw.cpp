#include "SDL2PXS.hpp"

// Private area
void SDL2PXS::drawGrid() {
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>();
    Uint32 color = SDL_MapRGB(surface->format, gridColor.R, gridColor.G, gridColor.B);

    for (int i = 0; i < PXSplane.pixelsInX; i++) {
        *rect = { (PXSize * (i + 1)) + (gridSize * i), 0, gridSize, height };
        SDL_FillRect(surface, rect.get(), color);
    }
    for (int i = 0; i < PXSplane.pixelsInY; i++) {
        *rect = SDL_Rect{ 0, (PXSize * (i + 1)) + (gridSize * i), width, gridSize };
        SDL_FillRect(surface, rect.get(), color);
    }
}

// Public area
void SDL2PXS::clearTheScreen() {
    SDL_RenderClear(renderer);
    S fill(PXSplane.pixels.begin(), PXSplane.pixels.end(), drawColor);
    if (gridSize > 0) SDL_RenderCopy(renderer, gridTexture, NULL, NULL);
}

void SDL2PXS::drawPixel(xy<int> pixel) {
    xy<int> stratPosInRealPixels = getStartOfPixelPos(pixel);
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(stratPosInRealPixels.x, stratPosInRealPixels.y, PXSize, PXSize);
    SDL_RenderFillRect(renderer, rect.get());

    setPixelColor(pixel);
}

void SDL2PXS::drawRect(SDL_Rect dst) {
    correctNegativeWH(dst);

    drawFillRect({ dst.x, dst.y, dst.w, 1 });
    drawFillRect({ dst.x, dst.y, 1, dst.h });
    drawFillRect({ dst.x, dst.y + (dst.h - 1) , dst.w, 1 });
    drawFillRect({ dst.x + (dst.w - 1), dst.y , 1, dst.h});
}

void SDL2PXS::drawFillRect(SDL_Rect dst) {
    correctNegativeWH(dst);

    xy<int> stratPosInRealPixels = getStartOfPixelPos({dst.x, dst.y});
    int widthInRealPixels = (dst.w * PXSize) + ((dst.w - 1) * gridSize),
        heightInRealPixels = (dst.h * PXSize) + ((dst.h - 1) * gridSize);
    S unique_ptr<SDL_Rect> rect = S make_unique<SDL_Rect>(stratPosInRealPixels.x, stratPosInRealPixels.y, widthInRealPixels, heightInRealPixels);
    SDL_RenderFillRect(renderer, rect.get());

    for (int i = 0; i < dst.h; i++) {
        for (int j = 0; j < dst.w; j++) {
            setPixelColor({ dst.x + j, dst.y + i });
        }
    }

    if (gridSize > 0) {
        *rect = SDL_Rect{ stratPosInRealPixels.x - gridSize, stratPosInRealPixels.y - gridSize, widthInRealPixels + gridSize, heightInRealPixels + gridSize };
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