#include "Widget.h"

bool Widget::handleEvent(const sf::Event& event) {
    int xPos, yPos;
    if (event.type == sf::Event::MouseButtonPressed) xPos = event.mouseButton.x, yPos = event.mouseButton.y;
    else if (event.type == sf::Event::MouseMoved) xPos = event.mouseMove.x, yPos = event.mouseMove.y;
    else return false;

    if (xPos >= x && xPos <= x + width && yPos >= y && yPos <= y + width) {
        if (event.type == sf::Event::MouseButtonPressed) mouseClicked();
        else if (event.type == sf::Event::MouseMoved) {
            if (!isMouseInside) {
                isMouseInside = true;
                mouseEntered();
            }
        }
        return true;
    }
    else if (isMouseInside) {
        isMouseInside = false;
        mouseExit();
        return true;
    }
    return false;
}

void Widget::setPosition(int x, int y) {
    this->x = x; this->y = y;
}

void Widget::setSize(int width, int height) {
    this->width = width; this->height = height;
}
