#ifndef MANDELBROTVIEWER_THEMESWITCHER_H
#define MANDELBROTVIEWER_THEMESWITCHER_H

#include "SFML/Graphics.hpp"
#include "ImageButton.h"
#include "../MandelbrotRenderer.h"

class ThemeSwitcher : public ImageButton {
public:
    bool mouseEntered() override;
    bool mouseExit() override;
    ThemeSwitcher(MandelbrotRenderer* renderer);

private:
    MandelbrotRenderer* renderer;
};


#endif //MANDELBROTVIEWER_THEMESWITCHER_H
