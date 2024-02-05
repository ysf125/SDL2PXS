#ifndef lineAlgorithm_hpp
#define lineAlgorithm_hpp
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <array>
#include <vector>
#define S std::

template <typename T>

struct xy { T x, y; };

float getAngle(xy<int> point0, xy<int> point1);

xy<int> movePointInGrid(xy<int> point, int dir, int D = 1);

S vector<xy<int>> angline(xy<int> point0, xy<int> point1);

#endif