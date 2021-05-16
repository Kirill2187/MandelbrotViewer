#include "GUI.h"

void GUI::addWidget(Widget* widget) {
    widgets.push_back(widget);
}

bool GUI::handleEvent(const sf::Event& event) {
    bool flag = false;
    for (Widget* widget : widgets)
        flag = widget->handleEvent(event) | flag;
    return flag;
}

void GUI::setTarget(sf::RenderTarget* target) {
//    this->target = target;
}

void GUI::draw() {
    for (Widget* widget : widgets)
        widget->draw(target);
}

GUI::GUI(sf::RenderTarget *target) {
    setTarget(target);
}
