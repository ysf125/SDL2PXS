#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <vector>
#define S std::

struct xy{ int x, y; };

struct RGBA{ Uint8 R, G, B, A; };

class SDL2PXS {
    uint32_t PXSize, width, height;
    RGBA backgroundColor, drawColor;
    SDL_Surface * surface;
    SDL_Texture * texture;

    float getStartOfPointPos(xy pointPos);

public:
    SDL2PXS(uint32_t PXSize, SDL_Window * window, SDL_Renderer * renderer, RGBA backgroundColor = {255, 255, 255, 255});

    SDL2PXS(uint32_t PXSize, uint32_t width, uint32_t height, RGBA backgroundColor = {255, 255, 255, 255});

    ~SDL2PXS();

    void setDrawColor(RGBA color = {0, 0, 0, 255});
    
    SDL_Window * getWindow();
    
    SDL_Renderer * getRenderer();

    RGBA getPixleColor(xy pixelPos);
    
    void drawPixel(xy pixelPos);
    
    void drawPixels(S vector<xy> pixelsPos);

    void drawRect(xy point0Pos, xy point1Pos);

    void drawLine(xy point0Pos, xy point1Pos);

    void drawCircle(xy centerPos, uint32_t R);

    void floodFill(xy pointPos);
};

#endif