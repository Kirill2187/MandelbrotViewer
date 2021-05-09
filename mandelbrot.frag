#version 130
#define BRIGHTNESS 1.8

uniform float maxIter = 512;
uniform float currentTheme = 1;
uniform vec4 frame; // cx, cy, sx, sy

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

    if (q == 0.0)       return vec4(rise, 0.0, 0.0, 1.0);
    else if (q == 1.0)  return vec4(1.0, rise, 0.0, 1.0);
    else if (q == 2.0)  return vec4(fall, 1.0, 0.0, 1.0);
    else if (q == 3.0)  return vec4(0.0, 1.0, rise, 1.0);
    else if (q == 4.0)  return vec4(0.0, fall, 1.0, 1.0);
    else if (q == 5.0)  return vec4(rise, 0.0, 1.0, 1.0);
    else if (q == 6.0)  return vec4(1.0, rise, 1.0, 1.0);

    return vec4(0, 0, 0, 1);
}

vec4 mandelbrot(float x, float y) {
    float x1 = (x - 0.5) * frame.z + frame.x, y1 = (y - 0.5) * frame.w + frame.y;
    float cx = x1, cy = y1;
    for (int i = 0; i < maxIter; ++i) {
        float xs = x1 * x1, ys = y1 * y1;
        if (xs + ys >= 4)
            return (currentTheme == 3 ? getColor2(i) : getColor(i));
        float x2 = xs - ys + cx;
        float y2 = 2 * x1 * y1 + cy;
        x1 = x2; y1 = y2;
    }
    return getColor(maxIter);
}

void main() {
    gl_FragColor = mandelbrot(gl_TexCoord[0].x, gl_TexCoord[0].y);
}
