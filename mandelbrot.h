#ifndef MANDELBROTVIEWER_MANDELBROT_H
#define MANDELBROTVIEWER_MANDELBROT_H

#include "SFML/Graphics/Color.hpp"
using namespace sf;

using ld = long double;
using ll = long long;

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY);

#endif