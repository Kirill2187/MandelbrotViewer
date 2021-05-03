#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector3.hpp"
using namespace sf;

using ld = long double;
using ll = long long;

int max_iter = 128;

void setMaxIter(int iter) {
    max_iter = iter;
}

Color getColor(int iter) {
    if (iter == max_iter)
        return Color::Black;
    ld q = (ld) iter / (ld) max_iter;
    if (q > 0.5) {
        return Color(q * 255, 255, q * 255);
    }
    else {
        return Color(0, q * 255, 0);
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

