#include "SFML/Graphics/Color.hpp"
using namespace sf;

using ld = long double;
using ll = long long;

const int MAX_ITER = 1024;

Color getColor(int iter) {
    if (iter == MAX_ITER)
        return Color::Black;
    ld q = (ld) iter / (ld) MAX_ITER;
    if (q > 0.4) {
        return Color(q * 255, 255, q * 255);
    }
    else {
        return Color(0, q * 255, 0);
    }
}

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY) {
    ld x1 = ((ld)x / windowWidth - 0.5) * sx + centerX, y1 = ((ld)y / windowHeight - 0.5) * sy + centerY;
    ld cx = x1, cy = y1;
    for (int i = 0; i < MAX_ITER; ++i) {
        ld xs = x1 * x1, ys = y1 * y1;
        if (xs + ys >= 4)
            return getColor(i);
        ld x2 = xs - ys + cx, y2 = 2 * x1 * y1 + cy;
        x1 = x2; y1 = y2;
    }
    return getColor(MAX_ITER);
}

