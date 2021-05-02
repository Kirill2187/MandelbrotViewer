#include "SFML/Graphics/Color.hpp"
using namespace sf;

using ld = long double;
using ll = long long;

const int MAX_ITER = 256;

Color getMatchingColor(int iter) {
    return (iter == MAX_ITER ? Color::Black : Color::White);
}

Color mandelbrot(int x, int y, int windowWidth, int windowHeight, ld sx, ld sy, ld centerX, ld centerY) {
    ld x1 = ((ld)x / windowWidth - 0.5) * sx + centerX, y1 = ((ld)y / windowHeight - 0.5) * sy + centerY;
    ld cx = x1, cy = y1;
    for (int i = 0; i < MAX_ITER; ++i) {
        ld xs = x1 * x1, ys = y1 * y1;
        if (xs + ys >= 4)
            return getMatchingColor(i);
        ld x2 = xs - ys + cx, y2 = 2 * x1 * y1 + cy;
        x1 = x2; y1 = y2;
    }
    return getMatchingColor(MAX_ITER);
}

