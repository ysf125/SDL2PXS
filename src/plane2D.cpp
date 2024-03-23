#include "SDL2PXS.hpp"
    
plane2D::plane2D(int pixelsInX, int pixelsInY)
:  pixelsInX(pixelsInX), pixelsInY(pixelsInY)  {
    pixels.resize(pixelsInY);
    for (int i = 0; i < pixelsInY; i++) pixels[i].resize(pixelsInX);
}

S tuple<int, int> plane2D::getPixelsInXAndY() {  return  {pixelsInX, pixelsInY}; }

void plane2D::clearThePlane(RGB drawColor) {
    for (int i = 0; i < pixelsInY; i++) S fill(pixels[i].begin(), pixels[i].end(), drawColor);
}

bool plane2D::notInsideThePlane(xy<int> pixel) {
    return (pixel.x < 0 || pixel.y < 0 || pixel.x > pixelsInX || pixel.y > pixelsInY);
}

RGB plane2D::getPixleColor(xy<int> pixel) {
    if (notInsideThePlane(pixel)) return { 0, 0, 0 };
    return pixels[pixel.y][pixel.x];
}
    
plane2D plane2D::copyFromPlane(SDL_Rect src) {
    correctNegativeWidthAndHeight(src);

    RGB color;
    plane2D returnedPlane = plane2D(src.w, src.h);
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            color = getPixleColor({ src.x + j, src.y + i });
            returnedPlane.drawPixel({ j, i }, color);
        }
    }
    return returnedPlane;
}

void plane2D::pasteToPlane(plane2D& srcPlane, SDL_Rect src, xy<int> dstStartPixel) {
    correctNegativeWidthAndHeight(src);

   RGB color;
    for (int i = 0; i < src.h; i++) {
        for (int j = 0; j < src.w; j++) {
            if (notInsideThePlane({ src.x + j, src.y + i })) continue;
            color = srcPlane.getPixleColor({ src.x + j, src.y + i });
            drawPixel({dstStartPixel.y + i, dstStartPixel.x + j}, color);
        }
    }
}

void plane2D::drawPixel(xy<int> pixel, RGB drawColor) {
    if (notInsideThePlane(pixel)) return;
    pixels[pixel.y][pixel.x] = drawColor;
}

void plane2D::drawFillRect(SDL_Rect dst, RGB drawColor) {
    for (int i = 0; i < dst.h; i++) {
        for (int j = 0; j < dst.w; j++) {
            drawPixel({ dst.x + j, dst.y + i }, drawColor);
        }
    }
}