#ifndef MANDELBROTVIEWER_GUI_H
#define MANDELBROTVIEWER_GUI_H

#include "Widget.h"
#include "iostream"
#include "SFML/Graphics.hpp"

class GUI {
private:
    std::vector<Widget*> widgets;
    sf::RenderTarget* target;
public:
    GUI(sf::RenderTarget* target);
    void addWidget(Widget* widget);
    bool handleEvent(const sf::Event& event);
    void setTarget(sf::RenderTarget* target);
    void draw();
};


#endif //MANDELBROTVIEWER_GUI_H
