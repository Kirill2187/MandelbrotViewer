#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "mandelbrot.h"
#include "TGUI/AllWidgets.hpp"
#include <TGUI/Backends/SFML/GuiSFML.hpp>

using namespace sf;
using ld = long double;

#define THEME_PATH "theme/Black.txt"
#define PANEL_SIZE 0.06

unsigned int width = 1000, height = 800;
unsigned int panel_height = height * PANEL_SIZE;
RenderWindow window;

bool isSelectionBoxActive = false;
Vector2i lastPressPosition;
const Color selectionBoxColor = Color(255, 50, 50);

Sprite mandelbrotImg;
Texture mandelbrotTexture;

ld cx = -0.5, cy = 0;
ld sx = 2.3, sy = sx * 0.8;

tgui::GuiSFML gui;
tgui::Theme mainTheme{THEME_PATH};

std::map<std::string, ColoringTheme> themes = {
        {"Green", GREEN},
        {"Red", RED},
        {"Blue", BLUE},
};
std::vector<int> iterations = {64, 128, 256, 512, 1024, 2048};

void calculateMandelbrot() {
    window.clear();
    gui.draw();
    window.display();

    Image img; img.create(width, height - panel_height);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height - panel_height; ++j) {
            img.setPixel(i, j, mandelbrot(i, j, width, height - panel_height, sx, sy, cx, cy));
        }
    }
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
        panel_height = height * PANEL_SIZE;
        sf::FloatRect visibleArea(0, 0, width, height);
        window.setView(sf::View(visibleArea));

        calculateMandelbrot();
    }
    else if (event.type == Event::MouseButtonPressed
    && event.mouseButton.y < height - panel_height) {
        isSelectionBoxActive = true;
        lastPressPosition = {event.mouseButton.x, event.mouseButton.y};
    }
    else if (event.type == Event::MouseButtonReleased) {
        if (isSelectionBoxActive && event.mouseButton.y < height - panel_height)
            zoom(lastPressPosition, {event.mouseButton.x, event.mouseButton.y});
        isSelectionBoxActive = false;
    }
    else if (event.type == Event::MouseMoved) {
        if (event.mouseMove.y >= height - panel_height)
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

void saveImage() {
    mandelbrotTexture.copyToImage().saveToFile("mandelbrot" + std::to_string(time(0)) + ".png");
}

void createPanel() {
    gui.setTarget(window);
    tgui::Layout panelHeight(std::to_string(int(PANEL_SIZE * 100)) + "%");
    tgui::Layout panelYPosition(std::to_string(100 - int(PANEL_SIZE * 100)) + "%");

    auto saveButton = tgui::Button::create("Save image");
    saveButton->setRenderer(mainTheme.getRenderer("Button"));
    saveButton->setSize("20%", panelHeight);
    saveButton->setTextSize(16);
    saveButton->setPosition(0, panelYPosition);
    saveButton->onPress(&saveImage);
    gui.add(saveButton);

    auto themeBox = tgui::ComboBox::create();
    themeBox->setRenderer(mainTheme.getRenderer("ComboBox"));
    themeBox->setSize("20%", panelHeight);
    themeBox->setPosition("22%", panelYPosition);
    for (const auto& element : themes) {
        themeBox->addItem(element.first);
    }
    themeBox->setTextSize(16);
    themeBox->onItemSelect([&] {
        auto box = gui.get<tgui::ComboBox>("themeBox");
        setTheme(themes[box->getSelectedItem().toStdString()]);
        calculateMandelbrot();
    });
    gui.add(themeBox, "themeBox");
    themeBox->setSelectedItem("Green");

    auto iterBox = tgui::ComboBox::create();
    iterBox->setRenderer(mainTheme.getRenderer("ComboBox"));
    iterBox->setSize("20%", panelHeight);
    iterBox->setPosition("44%", panelYPosition);
    for (const auto& element : iterations) {
        iterBox->addItem(std::to_string(element) + " iterations");
    }
    iterBox->setTextSize(16);
    iterBox->onItemSelect([&] {
        auto box = gui.get<tgui::ComboBox>("iterBox");
        setMaxIter(iterations[box->getSelectedItemIndex()]);
        calculateMandelbrot();
    });
    gui.add(iterBox, "iterBox");
    iterBox->setSelectedItemByIndex(2);
}

int main() {
    window.create(VideoMode(width, height), "Mandelbrot Viewer");
    window.setFramerateLimit(24);
    createPanel();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (gui.handleEvent(event)) {
                isSelectionBoxActive = false;
            }
            processEvent(event);
        }
        window.clear();
        window.draw(mandelbrotImg);
        gui.draw();

        if (isSelectionBoxActive)
            drawBox();

        window.display();
    }

    return 0;
}