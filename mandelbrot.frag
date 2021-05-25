#version 130
#define BRIGHTNESS 2

precision highp float;
precision highp int;

uniform float maxIter = 512;
uniform float currentTheme = 1;
uniform vec4 frame; // cx, cy, sx, sy

const int BASE = 10000;
const int N = 5;

struct big_float {
    int[N * 2 - 1] num;
    int sign;
};

big_float new_big_float() {
    big_float res; res.sign = 1;
    for (int i = 0; i < 2 * N - 1; ++i) res.num[i] = 0;
    return res;
}

big_float create(float a) {
    big_float res = new_big_float(); res.sign = a < 0 ? -1 : 1;
    a = abs(a);
    float mod = 1.0;
    for (int i = 0; i < N; ++i) {
        res.num[i] = int(floor(a / mod));
        a -= float(res.num[i]) * mod;
        if (a <= 0) return res;
        mod /= float(BASE);
    }

    return res;
}

bool greater(big_float a, big_float b) {
    if (a.sign == 1 && b.sign == -1) return true;
    if (a.sign == -1 && b.sign == 1) return false;
    bool res = a.sign == 1;
    for (int i = 0; i < N; ++i) {
        if (a.num[i] > b.num[i]) return res;
        else if (a.num[i] < b.num[i]) return !res;
    }
    return !res;
}

big_float subtract(big_float a, big_float b) {
    a.sign = 1; b.sign = 1;

    big_float res = new_big_float(); int carry = 0, sign = 1;
    if (greater(b, a)) sign = -1;
    res.sign = sign;

    if (sign == 1) {
        for (int i = N - 1; i >= 0; --i) {
            res.num[i] = a.num[i] - b.num[i] - carry;
            carry = 0;
            if (res.num[i] < 0) {
                res.num[i] += BASE; carry = 1;
            }
        }
    }
    else {
        for (int i = N - 1; i >= 0; --i) {
            res.num[i] = b.num[i] - a.num[i] - carry;
            carry = 0;
            if (res.num[i] < 0) {
                res.num[i] += BASE; carry = 1;
            }
        }
    }

    return res;
}

big_float add(big_float a, big_float b) {
    if (a.sign == 1 && b.sign == -1)
        return subtract(a, b);
    if (a.sign == -1 && b.sign == 1)
        return subtract(b, a);

    big_float res = new_big_float(); int carry = 0;
    res.sign = a.sign;
    for (int i = N - 1; i >= 0; --i) {
        res.num[i] = a.num[i] + b.num[i] + carry;
        carry = 0;
        if (res.num[i] >= BASE) {
            res.num[i] -= BASE; carry = 1;
        }
    }

    return res;
}

big_float multiply(big_float a, big_float b) {
    big_float res = new_big_float(); res.sign = a.sign * b.sign;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res.num[i + j] += a.num[i] * b.num[j];
            if (res.num[i + j] >= BASE) {
                res.num[i + j - 1] += res.num[i + j] / BASE;
                res.num[i + j] = int(mod(res.num[i + j], BASE));
            }
        }
    }

    for (int i = 2 * N - 2; i >= 0; --i) {
        if (res.num[i] >= BASE) {
            res.num[i - 1] += res.num[i] / BASE;
            res.num[i] = int(mod(res.num[i], BASE));
        }
    }
    return res;
}

vec4 getColor(float iter) {
    if (iter == maxIter)
        return vec4(0, 0, 0, 0);
    float q = iter / maxIter;
    if (q > 0.9) {
        float c1 = (-4 * q + 4) * BRIGHTNESS;
        float c2 = (-10 * q + 10);
        return vec4(currentTheme == 0 ? c2 : c1,
                    currentTheme == 1 ? c2 : c1,
                    currentTheme == 2 ? c2 : c1, 1);
    }
    else if (q > 0.5) {
        float c = (q - 0.5) * BRIGHTNESS;
        return vec4(currentTheme == 0 ? 1 : c,
                    currentTheme == 1 ? 1 : c,
                    currentTheme == 2 ? 1 : c, 1);
    }
    else {
        float c = q * 2;
        return vec4(currentTheme == 0 ? c : 0,
                    currentTheme == 1 ? c : 0,
                    currentTheme == 2 ? c : 0, 1);
    }
}

vec4 getColor2(float iter) {
    float q = iter / maxIter * 7.0;
    float rise = mod(q, 1.0);
    float fall = 1.0 - rise;
    q = floor(q);

    if (q == 0.0)       return vec4(rise, 0.0, 0.0, 1);
    else if (q == 1.0)  return vec4(1.0, rise, 0.0, 1);
    else if (q == 2.0)  return vec4(fall, 1.0, 0.0, 1);
    else if (q == 3.0)  return vec4(0.0, 1.0, rise, 1);
    else if (q == 4.0)  return vec4(0.0, fall, 1.0, 1);
    else if (q == 5.0)  return vec4(rise, 0.0, 1.0, 1);
    else if (q == 6.0)  return vec4(1.0, rise, 1.0, 1);

    return vec4(0, 0, 0, 1);
}

float getFloatValue(big_float a) {
    float res = 0;
    float b = 1.0;
    for (int i = 0; i < N; ++i) {
        res += float(a.num[i]) * b;
        b /= BASE;
    }
    return res * float(a.sign);
}


vec4 mandelbrot(float x, float y) {
    big_float four = create(4);
    big_float two = create(2);

    big_float x1 = (add(multiply(create(x - 0.5), create(frame.z)), create(frame.x))),
        y1 = (add(multiply(create(y - 0.5), create(frame.w)), create(frame.y)));
    big_float cx = x1, cy = y1;
    big_float px = cx, py = cy;
    for (int i = 0; i < maxIter; ++i) {
        big_float xs = (multiply(px, px)), ys = (multiply(py, py));
        if (greater(add(xs, ys), four))
            return (currentTheme == 3 ? getColor2(i) : getColor(i));

        py = add(cy, multiply(two, multiply(px, py)));
        px = add(cx, subtract(xs, ys));

    }
    return getColor(maxIter);
}

void main() {
    gl_FragColor = mandelbrot(gl_TexCoord[0].x, gl_TexCoord[0].y);
}
