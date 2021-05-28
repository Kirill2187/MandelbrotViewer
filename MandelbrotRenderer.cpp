#include "MandelbrotRenderer.h"

ColoringTheme MandelbrotRenderer::getCurrentTheme() const {
    return currentTheme;
}

void MandelbrotRenderer::setCurrentTheme(ColoringTheme currentTheme) {
    isBusy = true;
    MandelbrotRenderer::currentTheme = currentTheme;
    mandelbrotShader.setUniform("currentTheme", (float)currentTheme);
    updateImage();
    isBusy = false;
}

int MandelbrotRenderer::getNumberOfIterations() const {
    return numberOfIterations;
}

void MandelbrotRenderer::setNumberOfIterations(int numberOfIterations) {
    isBusy = true;
    if (numberOfIterations < MIN_NUMBER_OF_ITERATIONS) numberOfIterations = MIN_NUMBER_OF_ITERATIONS;
    if (numberOfIterations > MAX_NUMBER_OF_ITERATIONS) numberOfIterations = MAX_NUMBER_OF_ITERATIONS;
    MandelbrotRenderer::numberOfIterations = numberOfIterations;
    mandelbrotShader.setUniform("maxIter", (float)numberOfIterations);
    updateImage();
    isBusy = false;
}

void MandelbrotRenderer::setFrame(Frame newFrame) {
    isBusy = true;
    currentFrame = newFrame;
    float data[(N + 1) * 4];
    big_float arr[4] = {currentFrame.cx, currentFrame.cy, currentFrame.sx, currentFrame.sy};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < N; ++j) data[(N + 1) * i + j] = arr[i].num[j];
        data[(N + 1) * i + N] = arr[i].sign;
    }

    mandelbrotShader.setUniformArray("frameData", data, (N + 1) * 4);
    updateImage();
    isBusy = false;
}

Frame MandelbrotRenderer::getFrame() {
    return currentFrame;
}

void MandelbrotRenderer::updateImage() {
    mandelbrotTexture.create(width, height);
    mandelbrotTexture.clear(Color::Transparent);
    mandelbrotTexture.draw(emptySprite, &mandelbrotShader);
    mandelbrotTexture.display();

    mandelbrotSprite = Sprite(mandelbrotTexture.getTexture());
}

MandelbrotRenderer::MandelbrotRenderer(int width, int height, Frame frame) {
    create(width, height, frame);
}

void MandelbrotRenderer::create(int width, int height, Frame frame) {
    this->width = width;
    this->height = height;
    mandelbrotShader.loadFromFile(SHADER_PATH, Shader::Fragment);
    emptyTexture.create(width, height);
    emptySprite = Sprite(emptyTexture);
    mandelbrotShader.setUniform("maxIter", (float)numberOfIterations);
    mandelbrotShader.setUniform("currentTheme", (float)currentTheme);
    setFrame(frame);
}

const Sprite &MandelbrotRenderer::getMandelbrotSprite() const {
    return mandelbrotSprite;
}

bool MandelbrotRenderer::getIsBusy() const {
    return isBusy;
}


