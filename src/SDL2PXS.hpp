#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <vector>
#define S std::

struct RGB { Uint8 R, G, B; };

class SDL2PXS {
    int* width = new int, * height = new int;
    int PXSize, pixelsInX, pixelsInY, gridSize = 0;
    RGB drawColor, gridColor;
    S vector<RGB> pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;

    void setup();

    xy<int> getStartOfPixelPos(xy<int> pointPos);

    void drawGrid();

public:
    SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int PXSize, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    SDL2PXS(char title[], int pixelsInX, int pixelsInY, int PXSize, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    void closeSDL2PXS();

    SDL_Window* getWindow();

    SDL_Renderer* getRenderer();

    void showChanges();

    void setDrawColor(RGB color = { 0, 0, 0 });

    void clearTheScreen();

    RGB getPixleColor(xy<int> pixelPos);

    void drawPixel(xy<int> pixelPos);

    void drawFillRect(xy<int> startPointPos, int W, int H);

    void drawRect(xy<int> stratPointPos, int W, int H);

    void drawLine(xy<int> point0Pos, xy<int> point1Pos);

    void drawCircle(xy<int> centerPos, int R);

    void floodFill(xy<int> startPointPos);
};

#endif