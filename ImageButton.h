#ifndef MANDELBROTVIEWER_IMAGEBUTTON_H
#define MANDELBROTVIEWER_IMAGEBUTTON_H

#include "Widget.h"

class ImageButton : public Widget {
public:
    ImageButton(sf::String pathToTexture);
    bool mouseEntered() override;
    bool mouseExit() override;
    bool mouseClicked() override;
    void draw(sf::RenderTarget *target) override;
    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

private:
    void updateImagePosition();
    sf::Texture texture;
    sf::Sprite image;
};


#endif //MANDELBROTVIEWER_IMAGEBUTTON_H
