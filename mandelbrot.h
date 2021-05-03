#ifndef MANDELBROTVIEWER_MANDELBROT_H
#define MANDELBROTVIEWER_MANDELBROT_H

using ld = long double;

enum ColoringTheme {
    RED, GREEN, BLUE
};

struct Frame {
    ld cx, cy, sx, sy;
};

sf::Color mandelbrot(int x, int y, int windowWidth, int windowHeight, Frame frame);
void setMaxIter(int iter);
int getMaxIter();
ColoringTheme getTheme();
void setTheme(ColoringTheme theme);

#endif
