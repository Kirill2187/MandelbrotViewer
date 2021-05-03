#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "mandelbrot.h"
#include "TGUI/AllWidgets.hpp"
#include <iostream>

using namespace sf;
using ld = long double;

unsigned int width = 1000, height = 800;
RenderWindow window;

bool isSelectionBoxActive = false;
Vector2i lastPressPosition;
const Color selectionBoxColor = Color(255, 50, 50);

Sprite mandelbrotImg;
Texture mandelbrotTexture;

ld cx = -0.5, cy = 0;
ld sx = 2.3, sy = sx * 0.8;

void calculateMandelbrot() {
    Image img; img.create(width, height);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            img.setPixel(i, j, mandelbrot(i, j, width, height, sx, sy, cx, cy));
        }
    }
    img.saveToFile("mandelbrot.png");
    mandelbrotTexture.loadFromImage(img);
    mandelbrotImg = Sprite(mandelbrotTexture);
}

std::pair<ld, ld> screenToWorld(Vector2f p) {
    return {cx + sx * ((ld)p.x / width - 0.5),
            cy + sy * ((ld)p.y / height - 0.5)};
}

void zoom(Vector2i p1, Vector2i p2) {
    auto p = screenToWorld(((Vector2f)p1 + (Vector2f)p2) / 2.0f);
    cx = p.first; cy = p.second;
    sx = sx / width * abs(p1.x - p2.x);
    sy = sy / height * abs(p1.y - p2.y);

    calculateMandelbrot();
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

        calculateMandelbrot();
    }
    else if (event.type == Event::MouseButtonPressed) {
        isSelectionBoxActive = true;
        lastPressPosition = {event.mouseButton.x, event.mouseButton.y};
    }
    else if (event.type == Event::MouseMoved) {

    }
    else if (event.type == Event::MouseButtonReleased) {
        if (isSelectionBoxActive)
            zoom(lastPressPosition, {event.mouseButton.x, event.mouseButton.y});
        isSelectionBoxActive = false;
    }
    else if (event.type == sf::Event::LostFocus) {
        isSelectionBoxActive = false;
    }
}

void drawBox() {
    Vector2i mousePos = Mouse::getPosition(window);

    RectangleShape rect(Vector2f((mousePos.x - lastPressPosition.x),
                                 (mousePos.y - lastPressPosition.y)));
    rect.setFillColor(Color::Transparent);
    rect.setPosition(lastPressPosition.x, lastPressPosition.y);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(selectionBoxColor);
    window.draw(rect);
}

int main() {
    calculateMandelbrot();
    window.create(VideoMode(width, height), "Mandelbrot Viewer");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            processEvent(event);
        }
        window.clear();

        window.draw(mandelbrotImg);

        if (isSelectionBoxActive)
            drawBox();

        window.display();
    }

    return 0;
}