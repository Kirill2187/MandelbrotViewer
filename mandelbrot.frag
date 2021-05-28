#version 130
#define BRIGHTNESS 2
#define BUF_SIZE 9
#define N 5
#define BASE 10000

precision highp float;
precision highp int;

uniform float maxIter = 512;
uniform float currentTheme = 1;
uniform float[(N + 1) * 4] frameData;

vec4 frame; // cx, cy, sx, sy

struct big_float {
    int[N * 2 - 1] num;
    int sign;
};
big_float[BUF_SIZE] mem;

void reset_big_float(int num) {
    mem[num].sign = 1;
    for (int i = 0; i < 2 * N - 1; ++i) mem[num].num[i] = 0;
}

int copy(int from, int to) {
    for (int i = 0; i < N; ++i) mem[to].num[i] = mem[from].num[i];
    mem[to].sign = mem[from].sign;
    return to;
}

int create(float a, int to) {
    mem[to].sign = int(sign(a));
    a = abs(a);
    float mod = 1.0;
    for (int i = 0; i < N; ++i) {
        mem[to].num[i] = int(floor(a / mod));
        a -= float(mem[to].num[i]) * mod;
        mod /= float(BASE);
    }

    return to;
}

bool greater(int a, int b) {
    if (mem[a].sign != mem[b].sign) return mem[a].sign > mem[b].sign;
    bool res = mem[a].sign == 1;
    for (int i = 0; i < N; ++i) {
        if (mem[a].num[i] != mem[b].num[i]) return res ^^ !(mem[a].num[i] > mem[b].num[i]);
    }
    return !res;
}

int subtract(int a, int b, int to) {
    mem[a].sign = 1; mem[b].sign = 1;

    int carry = 0;
    int sign = int(greater(b, a)) * (-2) + 1;
    mem[to].sign = sign;

    for (int i = N - 1; i >= 0; --i) {
        mem[to].num[i] = sign * (mem[a].num[i] - mem[b].num[i]) - carry;
        carry = 0;
        if (mem[to].num[i] < 0) {
            mem[to].num[i] += BASE; carry = 1;
        }
    }

    return to;
}

int add(int a, int b, int to) {
    if (mem[a].sign == 1 && mem[b].sign == -1)
        return subtract(a, b, to);
    if (mem[a].sign == -1 && mem[b].sign == 1)
        return subtract(b, a, to);

    int carry = 0;
    mem[to].sign = mem[a].sign;
    for (int i = N - 1; i >= 0; --i) {
        mem[to].num[i] = mem[a].num[i] + mem[b].num[i] + carry;
        carry = 0;
        if (mem[to].num[i] >= BASE) {
            mem[to].num[i] -= BASE; carry = 1;
        }
    }

    return to;
}

int multiply(int a, int b, int to) {
    reset_big_float(to);
    mem[to].sign = mem[a].sign * mem[b].sign;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mem[to].num[i + j] += mem[a].num[i] * mem[b].num[j];
        }
    }

    for (int i = 2 * N - 2; i >= 0; --i) {
        if (mem[to].num[i] >= BASE) {
            mem[to].num[i - 1] += mem[to].num[i] / BASE;
            mem[to].num[i] = int(mod(mem[to].num[i], BASE));
        }
    }

    return to;
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

float getFloatValue(int a) {
    float res = 0;
    float b = 1.0;
    for (int i = 0; i < N; ++i) {
        res += float(mem[a].num[i]) * b;
        b /= BASE;
    }
    return res * float(mem[a].sign);
}


vec4 mandelbrot(float x, float y) {
    for (int i = 0; i < BUF_SIZE; ++i) reset_big_float(i);

    for (int i = 0; i < (N + 1) * 4; ++i) {
        int j = int(mod(i, N + 1));
        if (j < N) mem[i / (N + 1)].num[j] = int(frameData[i]);
        else mem[i / (N + 1)].sign = int(frameData[i]);
    }

    int x1 = (add(multiply(create(x - 0.5, 4), 2, 5), 0, 4)),
        y1 = (add(multiply(create(y - 0.5, 5), 3, 2), 1, 3));
    create(4, 8);

    /*
    0 - cx
    1 - cy
    2 - px
    3 - py
    4, 5, 6, 7 - buf
    8 - four
    */

    int cx = copy(4, 0), cy = copy(3, 1);
    int px = cx, py = cy;
    for (int i = 0; i < maxIter; ++i) {
        int xs = (multiply(px, px, 4)), ys = (multiply(py, py, 5));
        if (mem[add(xs, ys, 6)].num[0] >= 4)
            return (currentTheme == 3 ? getColor2(i) : getColor(i));

        int buf = multiply(px, py, 6);
        py = add(1, add(buf, buf, 7), 3);
        px = add(0, subtract(xs, ys, 6), 2);
    }
    return vec4(0, 0, 0, 1);
}

void main() {
    gl_FragColor = mandelbrot(gl_TexCoord[0].x, gl_TexCoord[0].y);
}
