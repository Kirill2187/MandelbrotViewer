#ifndef MANDELBROTVIEWER_MANDELBROT_H
#define MANDELBROTVIEWER_MANDELBROT_H

using namespace sf;

using ld = long double;
using ll = long long;

enum Theme {
    RED, GREEN, BLUE
};

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY);
void setMaxIter(int iter);
void setTheme(Theme theme);

#endif
