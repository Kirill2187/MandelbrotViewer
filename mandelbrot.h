#ifndef MANDELBROTVIEWER_MANDELBROT_H
#define MANDELBROTVIEWER_MANDELBROT_H

using ld = long double;

enum ColoringTheme {
    RED, GREEN, BLUE
};

sf::Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY);
void setMaxIter(int iter);
void setTheme(ColoringTheme theme);

#endif
