#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <stack>
#define S std::

enum options { none = 0, noOverflow = 1 };

struct RGB { Uint8 R, G, B; };
struct plane2D {
    int pixelsInX, pixelsInY;
    S vector<RGB> pixels;
};

class SDL2PXS {
    S unique_ptr<int> width = S make_unique<int>(), height = S make_unique<int>();
    int PXSize, gridSize;
    RGB drawColor, gridColor;
    options PXSOptions;
    plane2D PXSplane;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* gridTexture;

    void setup();

    xy<int> getStartOfPixelPos(xy<int> pixel);

    void setPixelColor(xy<int> pixel);

    void correctNegativeWH(xy<int>& startPixel, int& W, int& H);

    void drawGrid();

public:
    SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int pixelsInX, int pixelsInY, int PXSize, options PXSOptions = none, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    SDL2PXS(char windowTitle[], int pixelsInX, int pixelsInY, int PXSize, options PXSOptions = none, int gridSize = 0, RGB gridColor = { 0, 0, 0 }); // Not added yet

    //
    void closeSDL2PXS();

    //
    void getWidthAndHeight(int& W, int& H);

    //
    void getPixelsInXAndY(int& pixelsInX, int& pixelsInY);

    //
    void showChanges();

    // Clears the entire screen with one color and draws the grid if grid size > 0 
    void clearTheScreen();

    // Returns true if the given pixel isn't inside the screen
    bool notInsideTheScreen(xy<int> pixel);

    // Sets the color for anything that can be drawn to the screen
    void setDrawColor(RGB color = { 0, 0, 0 });

    // Returns RGB color for a pixel on the screen
    RGB getPixleColor(xy<int> pixel);

    // Not added yet 
    plane2D copyFromScreen(xy<int> startPixel, int W, int H);
    
    // Not added yet
    plane2D copyFromPlane(plane2D& plane, xy<int> startPixel, int W, int H);

    // Not added yet
    void pasteToScreen(plane2D& plane, SDL_Rect& src, SDL_Rect& dst); 

    // Not added yet
    void pasteToPlane(plane2D& srcPlane, plane2D& dstPlane, SDL_Rect& src, SDL_Rect& dst); 

    // Draws a pixel to the screen
    void drawPixel(xy<int> pixel);

    // Draws a rectangle to the screen
    void drawRect(xy<int> startPixel, int W, int H);

    // Draws a filled rectangle to the screen
    void drawFillRect(xy<int> startPixel, int W, int H);

    // Draws a line using my line algorithm (call it angline for now) to the screen
    void drawLine(xy<int> Pixel0, xy<int> pixel1);

    // Draws a circle using midpoint algorithm to the screen
    void drawCircle(xy<int> centerPixel, int R);

    // Draws a filled circle using midpoint algorithm to the screen
    void drawFillCircle(xy<int> centerPixel, int R);

    // fills any closed area using flood fill algorithm until it reaches the end of the screen 
    void fill(xy<int> startPixel, RGB oldColor);
};

#endif