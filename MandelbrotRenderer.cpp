#include "MandelbrotRenderer.h"

ColoringTheme MandelbrotRenderer::getCurrentTheme() const {
    return currentTheme;
}

void MandelbrotRenderer::setCurrentTheme(ColoringTheme currentTheme) {
    MandelbrotRenderer::currentTheme = currentTheme;
    mandelbrotShader.setUniform("currentTheme", (float)currentTheme);
    updateImage();
}

int MandelbrotRenderer::getNumberOfIterations() const {
    return numberOfIterations;
}

void MandelbrotRenderer::setNumberOfIterations(int numberOfIterations) {
    MandelbrotRenderer::numberOfIterations = numberOfIterations;
    mandelbrotShader.setUniform("maxIter", (float)numberOfIterations);
    updateImage();
}

void MandelbrotRenderer::setFrame(Frame newFrame) {
    currentFrame = newFrame;
    mandelbrotShader.setUniform("frame",Glsl::Vec4(currentFrame.cx, currentFrame.cy, currentFrame.sx, currentFrame.sy));
    updateImage();
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
    setFrame(frame);
}

const Sprite &MandelbrotRenderer::getMandelbrotSprite() const {
    return mandelbrotSprite;
}


