#include "SDL2PXS.hpp"

// private area

float SDL2PXS::getStartOfPixelPos(xy<int> pointPos) {

}

// public area

SDL2PXS::SDL2PXS(uint32_t PXSize, SDL_Window * window, SDL_Renderer * renderer) 
: PXSize(PXSize), window(window), renderer(renderer) {
    int * x, * y;
    SDL_GetWindowSizeInPixels(window, x, y);
    width = *x; height = *y;
    
}

SDL2PXS::SDL2PXS(uint32_t PXSize, uint32_t width, uint32_t height) {

}

SDL2PXS::SDL2PXS(uint32_t PXSize, uint32_t pixelsInX, uint32_t pixelsInY) {

}

SDL2PXS::~SDL2PXS() {

}
