#include "SDL2PXS.hpp"

// Public

// Utilities section

void SDL2PXS::restart() {
    plane2D plane = PXSplane;
    setup();
    for (int i = 0; i < PXSplane.pixelsInY; i++) {
        for (int j = 0; j < PXSplane.pixelsInX; j++) {
            setDrawColor(getPixleColorFromPlane(plane, { j, i }));
            drawPixel({ j, i });
        }
    }
}

bool SDL2PXS::notInsideTheScreen(xy<int> pixel) { return notInsideThePlane(PXSplane, pixel); }

bool SDL2PXS::notInsideThePlane(plane2D& plane, xy<int> pixel) {
    return (pixel.x < 0 || pixel.y < 0 || pixel.x > plane.pixelsInX || pixel.y > plane.pixelsInY);
}

// Setters and getters section

SDL_Window* SDL2PXS::getWindow() { return window; }

SDL_Renderer* SDL2PXS::getRenderer() { return renderer; }

S tuple<int, int> SDL2PXS::getWidthAndHeight() { return { width, height }; }

S tuple<int, int> SDL2PXS::getPixelsInXAndY() { return { PXSplane.pixelsInX, PXSplane.pixelsInY }; }

RGB SDL2PXS::getDrawColor() { return drawColor; }

options SDL2PXS::getPXSOptions() { return PXSOptions; }

RGB SDL2PXS::getPixleColor(xy<int> pixel) { return getPixleColorFromPlane(PXSplane, pixel); }

RGB SDL2PXS::getPixleColorFromPlane(plane2D& plane, xy<int> pixel) {
    if (notInsideThePlane(plane, pixel)) return { 0, 0, 0 };
    return plane.pixels[pixel.y][pixel.x];
}

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

void SDL2PXS::pasteToScreen(plane2D& plane, SDL_Rect src, xy<int> dstStartPixel) {
    correctNegativeWidthAndHeight(src);

    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            setDrawColor(getPixleColorFromPlane(plane, { src.x + j, src.y + i }));
            drawPixel({ dstStartPixel.x + j, dstStartPixel.y + i });
        }
    }
}

void SDL2PXS::pasteToPlane(plane2D& srcPlane, plane2D& dstPlane, SDL_Rect src, xy<int> dstStartPixel) {
    correctNegativeWidthAndHeight(src);

    RGB color;
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            if (notInsideThePlane(dstPlane, { src.x + j, src.y + i })) continue;
            color = getPixleColorFromPlane(srcPlane, { src.x + j, src.y + i });
            dstPlane.pixels[dstStartPixel.y + i][dstStartPixel.x + j] = color;
        }
    }
}

// Draw section

void SDL2PXS::drawPixel(xy<int> pixel) {
    xy<int> startPosInRealPixels = getStartOfPixelPos(pixel);
    SDL_RenderFillRect(renderer, S make_unique<SDL_Rect>(startPosInRealPixels.x, startPosInRealPixels.y, PXSize, PXSize).get());

    setPixelColor(pixel);
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

    for (int i = 0; i < dst.h; i++) {
        for (int j = 0; j < dst.w; j++) {
            setPixelColor({ dst.x + j, dst.y + i });
        }
    }

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