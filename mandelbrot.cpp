#include "SFML/Graphics/Color.hpp"
#include "mandelbrot.h"
using namespace sf;

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


