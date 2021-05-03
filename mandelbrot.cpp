#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector3.hpp"
#include "mandelbrot.h"
using namespace sf;

using ld = long double;
using ll = long long;

const int BRIGHTNESS = 120;

int max_iter = 128;
Theme currentTheme = GREEN;

void setMaxIter(int iter) {
    max_iter = iter;
}

void setTheme(Theme theme) {
    currentTheme = theme;
}

Color getColor(int iter) {
    if (iter == max_iter)
        return Color::Black;
    ld q = (ld) iter / (ld) max_iter;
    if (q > 0.5) {
        return Color(currentTheme == RED ? 255 : q * BRIGHTNESS,
                     currentTheme == GREEN ? 255 : q * BRIGHTNESS,
                     currentTheme == BLUE ? 255 : q * BRIGHTNESS);
    }
    else {
        return Color(currentTheme == RED ? q * 255 : 0,
                     currentTheme == GREEN ? q * 255 : 0,
                     currentTheme == BLUE ? q * 255 : 0);
    }
}

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY) {
    ld x1 = ((ld)x / windowWidth - 0.5) * sx + centerX, y1 = ((ld)y / windowHeight - 0.5) * sy + centerY;
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

