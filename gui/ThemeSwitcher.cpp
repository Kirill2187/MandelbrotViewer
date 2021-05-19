#include "ThemeSwitcher.h"

bool ThemeSwitcher::mouseEntered() {
    renderer->setNumberOfIterations(16);
    return true;
}

bool ThemeSwitcher::mouseExit() {
    renderer->setNumberOfIterations(32);
    return true;
}

ThemeSwitcher::ThemeSwitcher(MandelbrotRenderer *renderer) {
    setDrawButtonImage(false);
    this->renderer = renderer;
    renderer->updateImage();
    this->setImage(renderer->getMandelbrotSprite());
}

