#ifndef MANDELBROTVIEWER_MANDELBROTRENDERER_H
#define MANDELBROTVIEWER_MANDELBROTRENDERER_H

#define SHADER_PATH "mandelbrot.frag"

#include "SFML/Graphics.hpp"

using namespace sf;
using ld = long double;

struct Frame {
    ld cx, cy, sx, sy;
};

enum ColoringTheme {
    RED, GREEN, BLUE, RAINBOW
};

class MandelbrotRenderer {
private:
    int width, height;
    int numberOfIterations = 256;
    Sprite mandelbrotSprite;
    RenderTexture mandelbrotTexture;
    Frame currentFrame;
    ColoringTheme currentTheme = GREEN;
    Shader mandelbrotShader;
    Texture emptyTexture;
    Sprite emptySprite;
public:
    MandelbrotRenderer() = default;
    MandelbrotRenderer(int width, int height, Frame frame);

    int getNumberOfIterations() const;
    void setNumberOfIterations(int numberOfIterations);
    ColoringTheme getCurrentTheme() const;
    void setCurrentTheme(ColoringTheme currentTheme);
    const Sprite &getMandelbrotSprite() const;

    void setFrame(Frame newFrame);
    Frame getFrame();

    void updateImage();
    void create(int width, int height, Frame frame);
};


#endif //MANDELBROTVIEWER_MANDELBROTRENDERER_H
