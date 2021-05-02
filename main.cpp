#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "iostream"

using namespace sf;

unsigned int width = 800, height = 600;
RenderWindow window;

bool isBoxActive = false;
Vector2i lastPressPosition;

Color boxColor = Color(255, 50, 50);

void zoom(Vector2i p1, Vector2i p2) {

}

void processEvent(Event &event) {
    if (event.type == Event::Closed) {
        window.close();
    }
    else if (event.type == Event::Resized) {
        width = event.size.width;
        height = event.size.height;
        sf::FloatRect visibleArea(0, 0, width, height);
        window.setView(sf::View(visibleArea));
    }
    else if (event.type == Event::MouseButtonPressed) {
        isBoxActive = true;
        lastPressPosition = {event.mouseButton.x, event.mouseButton.y};
    }
    else if (event.type == Event::MouseMoved) {

    }
    else if (event.type == Event::MouseButtonReleased) {
        if (isBoxActive)
            zoom(lastPressPosition, {event.mouseButton.x, event.mouseButton.y});
        isBoxActive = false;
    }
    else if (event.type == sf::Event::LostFocus) {
        isBoxActive = false;
    }
}

void drawBox() {
    Vector2i mousePos = Mouse::getPosition(window);

    RectangleShape rect(Vector2f((mousePos.x - lastPressPosition.x),
                                 (mousePos.y - lastPressPosition.y)));
    rect.setFillColor(Color::Transparent);
    rect.setPosition(lastPressPosition.x, lastPressPosition.y);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(boxColor);
    window.draw(rect);
}

int main() {
    window.create(VideoMode(width, height), "Test window");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        window.clear();

        if (isBoxActive)
            drawBox();

        window.display();
    }

    return 0;
}