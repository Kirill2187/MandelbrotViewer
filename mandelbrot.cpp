#include "SFML/Graphics/Color.hpp"
#include "mandelbrot.h"
using namespace sf;

using ld = long double;

#define BRIGHTNESS 450

int max_iter = 256;
ColoringTheme currentTheme = GREEN;

void setMaxIter(int iter) {
    max_iter = iter;
}
int getMaxIter() {
    return max_iter;
}

void setTheme(ColoringTheme theme) {
    currentTheme = theme;
}
ColoringTheme getTheme() {
    return currentTheme;
}

Color getColor(int iter) {
    if (iter == max_iter)
        return Color::Black;
    ld q = (ld) iter / (ld) max_iter;
    if (q > 0.9) {
        unsigned int c = (-4 * q + 4) * BRIGHTNESS;
        unsigned int c1 = (-10 * q + 10);
        return Color(currentTheme == RED ? 255 * c1 : c,
                     currentTheme == GREEN ? 255 * c1 : c,
                     currentTheme == BLUE ? 255 * c1 : c);
    }
    else if (q > 0.5) {
        unsigned int c = (q - 0.5) * BRIGHTNESS;
        return Color(currentTheme == RED ? 255 : c,
                     currentTheme == GREEN ? 255 : c,
                     currentTheme == BLUE ? 255 : c);
    }
    else {
        unsigned int c = q * 510;
        return Color(currentTheme == RED ? c : 0,
                     currentTheme == GREEN ? c : 0,
                     currentTheme == BLUE ? c : 0);
    }
}

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, const Frame& frame) {
    ld x1 = ((ld)x / windowWidth - 0.5) * frame.sx + frame.cx;
    ld y1 = ((ld)y / windowHeight - 0.5) * frame.sy + frame.cy;
    ld cx = x1, cy = y1;
    for (int i = 0; i < max_iter; ++i) {
        ld xs = x1 * x1, ys = y1 * y1;
        if (xs + ys >= 4)
            return getColor(i);
        ld x2 = xs - ys + cx, y2 = 2 * x1 * y1 + cy;
        x1 = x2; y1 = y2;
    }
    return getColor(max_iter);
}

