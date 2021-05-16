#ifndef MANDELBROTVIEWER_WIDGET_H
#define MANDELBROTVIEWER_WIDGET_H

#include "SFML/Graphics.hpp"

class Widget {
public:
    bool handleEvent(const sf::Event& event);
    virtual bool mouseEntered() = 0;
    virtual bool mouseExit() = 0;
    virtual bool mouseClicked() = 0;
    virtual void draw(sf::RenderTarget* target) = 0;

    virtual void setSize(int width, int height);
    virtual void setPosition(int x, int y);

protected:
    int x, y, width, height;
    bool isMouseInside;
};


#endif //MANDELBROTVIEWER_WIDGET_H
