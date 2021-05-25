#include <cmath>
#include <cstring>
#include "BigFloat.h"

big_float add(big_float a, double b) {
    return add(a, create(b));
}

big_float add(big_float a, big_float b) {
    if (a.sign == 1 && b.sign == -1)
        return subtract(a, b);
    if (a.sign == -1 && b.sign == 1)
        return subtract(b, a);

    big_float res = {new int[N], a.sign}; int carry = 0;
    std::memset(res.num, 0, N * sizeof N);
    for (int i = N - 1; i >= 0; --i) {
        res.num[i] = a.num[i] + b.num[i] + carry;
        carry = 0;
        if (res.num[i] >= BASE) {
            res.num[i] -= BASE; carry = 1;
        }
    }
    if (carry == 1) throw std::overflow_error("Result is too big");

    return res;
}

big_float subtract(big_float a, big_float b) {
    a.sign = 1; b.sign = 1;
    big_float res = {new int[N], 1}; int carry = 0, sign = 1;
    std::memset(res.num, 0, N * sizeof N);

    if (greater(b, a)) sign = -1, std::swap(a, b);
    res.sign = sign;

    for (int i = N - 1; i >= 0; --i) {
        res.num[i] = a.num[i] - b.num[i] - carry;
        carry = 0;
        if (res.num[i] < 0) {
            res.num[i] += BASE; carry = 1;
        }
    }

    return res;
}

big_float multiply(big_float a, big_float b) {
    big_float res = {new int[2 * N - 1], a.sign * b.sign};
    memset(res.num, 0, (2 * N - 1) * sizeof(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res.num[i + j] += a.num[i] * b.num[j];
            if (res.num[i + j] >= BASE) {
                if (i + j == 0) throw std::overflow_error("Result is too big " + to_str(a) + " * " + to_str(b));
                res.num[i + j - 1] += res.num[i + j] / (int) BASE;
                if (res.num[i + j - 1] > 1e8) exit(-1);
                res.num[i + j] %= (int) BASE;
            }
        }
    }

    for (int i = 2 * N - 2; i >= 0; --i) {
        if (res.num[i] >= BASE) {
            if (i == 0) throw std::overflow_error("Result is too big " + to_str(a) + " * " + to_str(b));
            res.num[i - 1] += res.num[i] / (int) BASE;
            res.num[i] %= (int) BASE;
        }
    }


    return res;
}

big_float multiply(big_float a, double b) {
    return multiply(a, create(b));
}

big_float create(double a) {
    big_float res = {new int[N], a < 0 ? -1 : 1};
    a = std::abs(a);
    std::memset(res.num, 0, N * sizeof N);
    double mod = 1;
    for (int i = 0; i < N; ++i) {
        if (mod < 1e-12) break;
        res.num[i] = std::floor(a / mod);
        a -= res.num[i] * mod;
        if (a < 0) break;
        mod /= BASE;
    }

    return res;
}

std::string to_str(big_float a) {
    std::string res = a.sign == -1 ? "-" : "";
    res += std::to_string(a.num[0]) + ".";
    for (int i = 1; i < N; ++i) {
        std::string num = std::to_string(a.num[i]);
        res.append(std::string(4 - num.size(), '0') + num);
    }
    return res;
}

float getFloatValue(big_float a) {
    double res = 0;
    double b = 1.0;
    for (int i = 0; i < N; ++i) {
        res += a.num[i] * b;
        b /= BASE;
    }
    return res * a.sign;
}

bool greater(big_float a, big_float b) {
    if (a.sign == 1 && b.sign == -1) return 1;
    if (a.sign == -1 && b.sign == 1) return 0;
    bool res = a.sign == 1;
    for (int i = 0; i < N; ++i) {
        if (a.num[i] > b.num[i]) return res;
        else if (a.num[i] < b.num[i]) return !res;
    }
    return !res;
}

