#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "mandelbrot.h"
#include "TGUI/AllWidgets.hpp"
#include <TGUI/Backends/SFML/GuiSFML.hpp>
#include "thread"
#include "mutex"
#include "iostream"

using namespace sf;
using ld = long double;

#define THEME_PATH "theme/Black.txt"
#define PANEL_SIZE 0.06

unsigned int width = VideoMode::getDesktopMode().width, height = VideoMode::getDesktopMode().height;
unsigned int panelHeight = height * PANEL_SIZE;
RenderWindow window;

bool isSelectionBoxActive = false;
Vector2i lastPressPosition;
const Color selectionBoxColor = Color(255, 50, 50);

Sprite mandelbrotImg;
RenderTexture mandelbrotTexture;

const Frame startFrame = {-0.5, 0, 2.3, 2.3 * 0.8};
Frame currentFrame = startFrame;
std::vector<Frame> framesStack = {startFrame};

tgui::GuiSFML gui;
tgui::Theme mainTheme{THEME_PATH};

std::map<std::string, ColoringTheme> themes = {
        {"Green", GREEN},
        {"Red", RED},
        {"Blue", BLUE},
        {"Rainbow", RAINBOW},
};
std::vector<int> iterations = {64, 128, 256, 512, 1024, 2048, 10000};

Shader mandelbrotShader;

void updateShader() {
    mandelbrotShader.setUniform("frame", Glsl::Vec4(currentFrame.cx, currentFrame.cy, currentFrame.sx, currentFrame.sy));
    mandelbrotShader.setUniform("maxIter", static_cast<float>(getMaxIter()));
    mandelbrotShader.setUniform("currentTheme", static_cast<float>(getTheme()));
}

Texture emptyTexture;
Sprite emptySprite;

void updateMandelbrot() {
    updateShader();

    emptyTexture.create(width, height - panelHeight);
    emptySprite = Sprite(emptyTexture);

    mandelbrotTexture.create(width, height - panelHeight);
    mandelbrotTexture.clear(Color::Black);
    mandelbrotTexture.draw(emptySprite, &mandelbrotShader);
    mandelbrotTexture.display();

    mandelbrotImg = Sprite(mandelbrotTexture.getTexture());
}

std::pair<ld, ld> screenToWorld(Vector2f p) {
    return {currentFrame.cx + currentFrame.sx * ((ld)p.x / width - 0.5),
            currentFrame.cy + currentFrame.sy * ((ld)p.y / (height - panelHeight) - 0.5)};
}

void zoom(Vector2i p1, Vector2i p2) {
    auto p = screenToWorld(((Vector2f)p1 + (Vector2f)p2) / 2.0f);
    ld cx = p.first; ld cy = p.second;
    ld sx = currentFrame.sx / width * abs(p1.x - p2.x);
    ld sy = currentFrame.sy / (height - panelHeight) * abs(p1.y - p2.y);
    framesStack.push_back({cx, cy, sx, sy});
    currentFrame = framesStack.back();

    updateMandelbrot();
}

void revertFrame() {
    if (framesStack.size() <= 1)
        return;
    framesStack.pop_back();
    currentFrame = framesStack.back();

    updateMandelbrot();
}

void processEvent(Event &event) {
    if (event.type == Event::Closed) {
        window.close();
    }
    else if (event.type == Event::Resized) {
        width = event.size.width;
        height = event.size.height;
        panelHeight = height * PANEL_SIZE;
        sf::FloatRect visibleArea(0, 0, width, height);
        window.setView(sf::View(visibleArea));

        updateMandelbrot();
    }
    else if (event.type == Event::MouseButtonPressed
    && event.mouseButton.y < height - panelHeight) {
        isSelectionBoxActive = true;
        lastPressPosition = {event.mouseButton.x, event.mouseButton.y};
    }
    else if (event.type == Event::MouseButtonReleased) {
        if (isSelectionBoxActive && event.mouseButton.y < height - panelHeight)
            zoom(lastPressPosition, {event.mouseButton.x, event.mouseButton.y});
        isSelectionBoxActive = false;
    }
    else if (event.type == Event::MouseMoved) {
        if (event.mouseMove.y >= height - panelHeight)
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
    mandelbrotTexture.getTexture().copyToImage().saveToFile("mandelbrot" + std::to_string(time(0)) + ".png");
}

void createPanel() {
    auto downloadTexture = new tgui::Texture(); downloadTexture->load("images/download.png");
    auto homeTexture = new tgui::Texture(); homeTexture->load("images/home.png");
    auto returnTexture = new tgui::Texture(); returnTexture->load("images/return.png");
    auto crossTexture = new tgui::Texture(); crossTexture->load("images/cross.png");

    gui.setTarget(window);
    tgui::Layout panelHeight(std::to_string(int(PANEL_SIZE * 100)) + "%");
    tgui::Layout panelYPosition(std::to_string(100 - int(PANEL_SIZE * 100)) + "%");

    auto saveButton = tgui::BitmapButton::create();
    saveButton->setImageScaling(1);
    saveButton->setImage(*downloadTexture);
    saveButton->setRenderer(mainTheme.getRenderer("Button"));
    saveButton->setSize("10%", panelHeight);
    saveButton->setPosition(0, panelYPosition);
    saveButton->onPress(&saveImage);
    gui.add(saveButton);

    auto revertButton = tgui::BitmapButton::create();
    revertButton->setImageScaling(1);
    revertButton->setImage(*returnTexture);
    revertButton->setRenderer(mainTheme.getRenderer("Button"));
    revertButton->setSize("10%", panelHeight);
    revertButton->setPosition("11%", panelYPosition);
    revertButton->onPress([&] {
        revertFrame();
    });
    gui.add(revertButton);

    auto quitButton = tgui::BitmapButton::create();
    quitButton->setImageScaling(1);
    quitButton->setImage(*crossTexture);
    quitButton->setRenderer(mainTheme.getRenderer("Button"));
    quitButton->setSize("10%", panelHeight);
    quitButton->setPosition("90%", panelYPosition);
    quitButton->onPress([&] {
        window.close();
    });
    gui.add(quitButton);

    auto homeButton = tgui::BitmapButton::create();
    homeButton->setImageScaling(1);
    homeButton->setImage(*homeTexture);
    homeButton->setRenderer(mainTheme.getRenderer("Button"));
    homeButton->setSize("10%", panelHeight);
    homeButton->setPosition("22%", panelYPosition);
    homeButton->onPress([&] {
        framesStack.push_back(startFrame);
        currentFrame = startFrame;
        updateMandelbrot();
    });
    gui.add(homeButton);

    auto themeBox = tgui::ComboBox::create();
    themeBox->setRenderer(mainTheme.getRenderer("ComboBox"));
    themeBox->setSize("12%", panelHeight);
    themeBox->setPosition("43%", panelYPosition);
    for (const auto& element : themes) {
        themeBox->addItem(element.first);
    }
    themeBox->setTextSize(16);
    themeBox->onItemSelect([&] {
        auto box = gui.get<tgui::ComboBox>("themeBox");
        auto prevTheme = getTheme();
        setTheme(themes[box->getSelectedItem().toStdString()]);
        if (prevTheme != getTheme()) updateMandelbrot();
    });
    gui.add(themeBox, "themeBox");
    themeBox->setSelectedItem("Green");

    auto iterBox = tgui::ComboBox::create();
    iterBox->setRenderer(mainTheme.getRenderer("ComboBox"));
    iterBox->setSize("20%", panelHeight);
    iterBox->setPosition("56%", panelYPosition);
    for (const auto& element : iterations) {
        iterBox->addItem(std::to_string(element) + " iterations");
    }
    iterBox->setTextSize(16);
    iterBox->onItemSelect([&] {
        auto box = gui.get<tgui::ComboBox>("iterBox");
        int prevIter = getMaxIter();
        setMaxIter(iterations[box->getSelectedItemIndex()]);
        if (prevIter != getMaxIter()) updateMandelbrot();
    });
    gui.add(iterBox, "iterBox");
    iterBox->setSelectedItemByIndex(2);
}

int main() {
    if (!Shader::isAvailable()) {
        std::cout << "Shaders is not available!" << std::endl;
        exit(0);
    }

    mandelbrotShader.loadFromFile("mandelbrot.frag", Shader::Fragment);

    window.create(VideoMode::getFullscreenModes()[0], "Mandelbrot Viewer", Style::Fullscreen);
    window.setFramerateLimit(24);
    createPanel();
    updateMandelbrot();

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