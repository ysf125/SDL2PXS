#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <vector>
#include <cmath>
#include <memory>
#include <string>
#include <stack>
#define S std::

enum options { none = 0, resizeTheScreen = 1 };

struct RGB { Uint8 R, G, B; };

class SDL2PXS {
    S unique_ptr<int> width = S make_unique<int>(), height = S make_unique<int>();
    int PXSize, pixelsInX, pixelsInY, gridSize;
    RGB drawColor, gridColor;
    options PXSOptions;
    S vector<RGB> pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;

    void setup();

    xy<int> getStartOfPixelPos(xy<int> pixel);

    void setPixelColor(xy<int> pixel);

    void drawGrid();

public:
    SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions = none, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    void closeSDL2PXS();

    void getWidthAndHeight(int* width, int* height);

    void getPixelsInXAndY(int* pixelsInX, int* pixelsInY);

    void showChanges();

    void clearTheScreen();

    bool notInsideTheScreen(xy<int> pixel);

    void setDrawColor(RGB color = { 0, 0, 0 });

    RGB getPixleColor(xy<int> pixel);

    void drawPixel(xy<int> pixel);

    void drawRect(xy<int> startPixel, int W, int H);

    void drawFillRect(xy<int> startPixel, int W, int H);

    void drawLine(xy<int> Pixel0, xy<int> pixel1);

    void drawCircle(xy<int> centerPixel, int R);

    void drawFillCircle(xy<int> centerPixel, int R);

    void floodFill(xy<int> startPixel, RGB oldColor);
};

#endif