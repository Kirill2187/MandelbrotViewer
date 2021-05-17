#ifndef MANDELBROTVIEWER_IMAGEBUTTON_H
#define MANDELBROTVIEWER_IMAGEBUTTON_H

#include "Widget.h"

class ImageButton : public Widget {
public:
    ImageButton(sf::String pathToTexture);
    bool mouseEntered() override;
    bool mouseExit() override;
    bool mouseClicked() override;
    void draw(sf::RenderTarget*) override;
    void setPosition(int, int) override;
    bool mousePressed() override;

    void setSize(int, int) override;
    void setOnClick(void (*func) ());
private:
    sf::Sprite buttonUpSprite, buttonDownSprite, buttonSelectedSprite;
    void updateImagePosition();
    static void resizeSprite(sf::Sprite&, float, float);
    sf::Texture texture;
    sf::Sprite image;
    void (*onClick) ();
};


#endif //MANDELBROTVIEWER_IMAGEBUTTON_H
