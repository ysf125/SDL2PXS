#ifndef SDL2PXS_hpp
#define SDL2PXS_hpp
#include <SDL.h>
#include <lineAlgorithm.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <cmath>
#include <memory>
#include <stack>
#define S std::

enum options { none = 0, resizeTheScreen = 1, autoWidthAndHeight = 2, autoPixelsInXAndY = 4 };

struct RGB { Uint8 R, G, B; };

struct plane2D {
    int pixelsInX, pixelsInY;
    S vector<S vector<RGB>> pixels;
};

class SDL2PXS {
    xy<int> startPixel;
    int width, height;
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

    void correctNegativeWidthAndHeight(SDL_Rect& rect);

    void drawGrid();

public:
// Main section

    SDL2PXS(SDL_Window* window, SDL_Renderer* renderer, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
    options PXSOptions = none, xy<int> startPixel = { 0, 0 }, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    SDL2PXS(S string windowTitle, int W, int H, int pixelsInX, int pixelsInY, int PXSize,
    options PXSOptions = none, xy<int> startPixel = { 0, 0 }, int gridSize = 0, RGB gridColor = { 0, 0, 0 });

    // You can call this method when you finish using SDL2PXS 
    void closeSDL2PXS();

    // Shows whatever is drawn to the screen 
    void showChanges();

    // Clears the entire screen and draws the grid if grid size > 0 
    void clearTheScreen();

// Utilities section

    // Restart everything without deleting the pixels or the gird
    void restartEverything();

    // Returns true if the given pixel isn't inside the screen
    bool notInsideTheScreen(xy<int> pixel);

    // Returns true if the given pixel isn't inside the plane given
    bool notInsideThePlane(plane2D& plane, xy<int> pixel);

// Setters and getters section

    // Returns the window
    SDL_Window* getWindow();

    // Returns the renderer
    SDL_Renderer* getRenderer();

    // Returns width and height in real pixels  
    S tuple<int, int> getWidthAndHeight();

    // Returns the number of pixels in the x-axis and y-axis
    S tuple<int, int> getPixelsInXAndY();

    // Returns draw color
    RGB getDrawColor();

    // Returns the options for the screen 
    options getPXSOptions();

    // Sets the start pixel (0, 0) for the given pixel in real pixels 
    void setStartPixel(xy<int> startPixel = { 0, 0 });

    // Sets the color for anything that can draw to the screen
    void setDrawColor(RGB color = { 0, 0, 0 });

    // Returns RGB color for a pixel on the screen
    RGB getPixleColor(xy<int> pixel);

    // Returns RGB color for a pixel on the plane given
    RGB getPixleColorFromPlane(plane2D& plane, xy<int> pixel);

// Copy and paste section

    // Copies a rectangle of pixels from the screen to plane2D struct
    plane2D copyFromScreen(SDL_Rect src);

    // Copies a rectangle of pixels from plane2D struct to the screen
    plane2D copyFromPlane(plane2D& plane, SDL_Rect src);

    // Pastes a rectangle of pixels from plane2D struct to the screen
    void pasteToScreen(plane2D& plane, SDL_Rect src, xy<int> dstStartPixel);

    // Pastes a rectangle of pixels from plane2D struct to another plane2D struct 
    void pasteToPlane(plane2D& srcPlane, plane2D& dstPlane, SDL_Rect src, xy<int> dstStartPixel);

// Draw section

    // Draws a pixel to the screen
    void drawPixel(xy<int> pixel);

    // Draws a rectangle to the screen
    void drawRect(SDL_Rect dst);

    // Draws a filled rectangle to the screen
    void drawFillRect(SDL_Rect dst);

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