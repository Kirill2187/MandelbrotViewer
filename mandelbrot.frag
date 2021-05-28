#version 130
#define BRIGHTNESS 2

precision highp float;
precision highp int;

const int BASE = 10000;
const int N = 5;

uniform float maxIter = 512;
uniform float currentTheme = 1;
uniform float[(N + 1) * 4] frameData;

vec4 frame; // cx, cy, sx, sy

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
    big_float res = new_big_float(); res.sign = int(sign(a));
    a = abs(a);
    float mod = 1.0;
    for (int i = 0; i < N; ++i) {
        res.num[i] = int(floor(a / mod));
        a -= float(res.num[i]) * mod;
        mod /= float(BASE);
    }

    return res;
}

bool greater(big_float a, big_float b) {
    bool res = a.sign == 1;
    for (int i = 0; i < N; ++i) {
        if (a.num[i] == b.num[i]) continue;
        return res ^^ !(a.num[i] > b.num[i]);
    }
    return !res;
}

big_float subtract(big_float a, big_float b) {
    a.sign = 1; b.sign = 1;

    big_float res = new_big_float(); int carry = 0;
    int sign = int(greater(b, a)) * (-2) + 1;
    res.sign = sign;

    for (int i = N - 1; i >= 0; --i) {
        res.num[i] = sign * (a.num[i] - b.num[i]) - carry;
        carry = 0;
        if (res.num[i] < 0) {
            res.num[i] += BASE; carry = 1;
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

    big_float arr[4];
    for (int i = 0; i < (N + 1) * 4; ++i) {
        int j = int(mod(i, N + 1));
        if (j < N) arr[i / (N + 1)].num[j] = int(frameData[i]);
        else arr[i / (N + 1)].sign = int(frameData[i]);
    }

    big_float x1 = (add(multiply(create(x - 0.5), arr[2]), arr[0])),
        y1 = (add(multiply(create(y - 0.5), arr[3]), arr[1]));
    big_float cx = x1, cy = y1;
    big_float px = cx, py = cy;
    big_float xs, ys, buf;
    for (int i = 0; i < maxIter; ++i) {
        xs = (multiply(px, px)), ys = (multiply(py, py));
        if (greater(add(xs, ys), four))
            return (currentTheme == 3 ? getColor2(i) : getColor(i));

        buf = multiply(px, py);
        py = add(cy, add(buf, buf));
        px = add(cx, subtract(xs, ys));

    }
    return vec4(0, 0, 0, 1);;
}

void main() {
    gl_FragColor = mandelbrot(gl_TexCoord[0].x, gl_TexCoord[0].y);
}
