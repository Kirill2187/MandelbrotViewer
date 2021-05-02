#include "SFML/Window.hpp"
#include "iostream"

using namespace sf;

int width = 800, height = 600;

int main() {

    Window window(VideoMode(width, height), "Test window");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::Resized) {
                width = event.size.width;
                height = event.size.height;
            }
            else if (event.type == Event::MouseButtonPressed) {
                std::cout << "Mouse button pressed: ";
                std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
            }
            else if (event.type == Event::MouseMoved) {
                std::cout << "Mouse button moved: ";
                std::cout << event.mouseMove.x << " " << event.mouseMove.y << std::endl;
                if (Mouse::isButtonPressed(sf::Mouse::Left)) {

                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                std::cout << "Mouse button released: ";
                std::cout << event.mouseButton.x << " " << event.mouseButton.y << std::endl;
            }
            else if (event.type == sf::Event::LostFocus) {

            }
        }
    }

    return 0;
}