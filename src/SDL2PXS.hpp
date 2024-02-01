#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <vector>
#define S std::

struct RGBA{ Uint8 R, G, B, A; };

class SDL2PXS {
    uint32_t PXSize, width, height, pixelsInX, pixelsInY;
    RGBA drawColor;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * surface;
    SDL_Texture * texture;

    float getStartOfPixelPos(xy<int> pointPos);

public:
    SDL2PXS(uint32_t PXSize, SDL_Window * window, SDL_Renderer * renderer);

    SDL2PXS(uint32_t PXSize, uint32_t width, uint32_t height);

    SDL2PXS(uint32_t PXSize, uint32_t pixelsInX, uint32_t pixelsInY);

    ~SDL2PXS();
    
    SDL_Window * getWindow();
    
    SDL_Renderer * getRenderer();
    
    void setDrawColor(RGBA color = {0, 0, 0, 255});

    RGBA getPixleColor(xy<int> pixelPos);
    
    void drawPixel(xy<int> pixelPos);
    
    void drawPixels(S vector<xy<int>> pixelsPos);

    void drawRect(xy<int> point0Pos, xy<int> point1Pos);

    void drawLine(xy<int> point0Pos, xy<int> point1Pos);

    void drawCircle(xy<int> centerPos, uint32_t R);

    void floodFill(xy<int> pointPos);

    void clearTheScreen();
};

#endif