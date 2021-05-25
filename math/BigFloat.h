#ifndef MANDELBROTVIEWER_BIGFLOAT_H
#define MANDELBROTVIEWER_BIGFLOAT_H

#include "iostream"
#include <ctgmath>
#include "SFML/Graphics/Glsl.hpp"
#include <iomanip>

const double BASE = 1e4;
const int N = 5;

struct big_float {
    int* num;
    int sign;
};

big_float add(big_float a, double b);
big_float add(big_float a, big_float b);
big_float subtract(big_float a, big_float b);

big_float multiply(big_float a, big_float b);
big_float multiply(big_float a, double b);

big_float create(double a);
std::string to_str(big_float a);

bool greater(big_float a, big_float b);

float getFloatValue(big_float a);

#endif //MANDELBROTVIEWER_BIGFLOAT_H
