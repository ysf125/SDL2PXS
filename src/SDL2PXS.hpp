#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <iostream>
#include <vector>

#define S std::

struct RGB { Uint8 R, G, B; };

class SDL2PXS {
    int* width = new int, * height = new int;
    int PXSize, pixelsInX, pixelsInY;
    RGB drawColor = { 0, 0, 0 };
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;

    void setup();

    xy<float> getStartOfPixelPos(xy<int> pointPos);

public:
    SDL2PXS(int PXSize, SDL_Window* window, SDL_Renderer* renderer);

    SDL2PXS(int PXSize, int pixelsInX, int pixelsInY);

    ~SDL2PXS();

    SDL_Window* getWindow();

    SDL_Renderer* getRenderer();

    void showChanges();

    void setDrawColor(RGB color = { 0, 0, 0 });

    void clearTheScreen();

    RGB getPixleColor(xy<int> pixelPos);

    void drawPixel(xy<int> pixelPos);

    void drawRect(xy<int> point0Pos, xy<int> point1Pos);
    
    void drawFillRect(xy<int> point0Pos, xy<int> point1Pos);

    void drawLine(xy<int> point0Pos, xy<int> point1Pos);

    void drawCircle(xy<int> centerPos, int R);

    void floodFill(xy<int> pointPos);
};

#endif