#ifndef MANDELBROTVIEWER_MANDELBROT_H
#define MANDELBROTVIEWER_MANDELBROT_H

using ld = long double;

enum ColoringTheme {
    RED, GREEN, BLUE
};

struct Frame {
    ld cx, cy, sx, sy;
};

void setMaxIter(int iter);
int getMaxIter();
ColoringTheme getTheme();
void setTheme(ColoringTheme theme);

#endif
