#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "iostream"
#include "gui/GUI.h"
#include "gui/ThemeSwitcher.h"
#include "gui/ImageButton.h"
#include "MandelbrotRenderer.h"

using namespace sf;
using ld = long double;

#define PANEL_SIZE 0.06
#define BUTTON_WIDTH 150
#define MOUSE_WHEEL_SENSITIVITY 30

const int WIDTH = VideoMode::getDesktopMode().width, HEIGHT = VideoMode::getDesktopMode().height;
const unsigned int PANEL_HEIGHT = static_cast<unsigned int>(HEIGHT * PANEL_SIZE);
const int IMAGE_HEIGHT = HEIGHT - PANEL_HEIGHT;

const Color SELECTION_BOX_COLOR = Color(255, 50, 50);
const Frame START_FRAME = {-0.5, 0, 4.0, 4.0 * (HEIGHT - PANEL_HEIGHT) / WIDTH};

RenderWindow window;
GUI gui(&window);
MandelbrotRenderer mainRenderer, themeSwitcherRenderer;
bool isSelectionBoxActive = false;
Vector2f lastPressPosition;
float mouseWheelRotationPerFrame = 0;

Frame currentFrame = START_FRAME;
std::vector<Frame> framesStack = {START_FRAME};

std::pair<ld, ld> screenToWorld(Vector2f p) {
    return {currentFrame.cx + currentFrame.sx * ((ld)p.x / WIDTH - 0.5),
            currentFrame.cy + currentFrame.sy * ((ld)p.y / IMAGE_HEIGHT - 0.5)};
}

Vector2f makeSelectionBoxCorrect(Vector2f sz) {
    float r = (float) WIDTH / (float) IMAGE_HEIGHT;
    if (std::abs(sz.x / sz.y) > r) {
        sz.x = sz.x / std::abs(sz.x) * std::abs(sz.y) * r;
    }
    else {
        sz.y = sz.y / std::abs(sz.y) * std::abs(sz.x) / r;
    }
    return sz;
}

void zoom(Vector2f p1, Vector2f p2) {
    auto p = screenToWorld((p1 + p2) / 2.0f);
    float r = (float) WIDTH / IMAGE_HEIGHT;

    ld cx = p.first; ld cy = p.second;
    ld sx = currentFrame.sx / WIDTH * abs(p1.x - p2.x);
    ld sy = currentFrame.sy / IMAGE_HEIGHT * abs(p1.y - p2.y);
    if (sy > sx / r) {
        sx = sy * r;
    }
    else if (sx > sy * r) {
        sy = sx / r;
    }

    framesStack.push_back({cx, cy, sx, sy});
    currentFrame = framesStack.back();
    mainRenderer.setFrame(currentFrame);
}

void revertFrame() {
    if (framesStack.size() <= 1)
        return;
    framesStack.pop_back();
    currentFrame = framesStack.back();
    mainRenderer.setFrame(currentFrame);
}

void processEvent(Event &event) {
    if (event.type == Event::Closed) {
        window.close();
    }
    else if (event.type == Event::MouseButtonPressed
    && event.mouseButton.y < IMAGE_HEIGHT) {
        isSelectionBoxActive = true;
        lastPressPosition = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};
    }
    else if (event.type == Event::MouseButtonReleased) {
        if (isSelectionBoxActive && event.mouseButton.y < IMAGE_HEIGHT) {
//            Vector2f sz = makeSelectionBoxCorrect(Vector2f((event.mouseButton.x - lastPressPosition.x),(event.mouseButton.y - lastPressPosition.y)));
            Vector2f sz = Vector2f((event.mouseButton.x - lastPressPosition.x),(event.mouseButton.y - lastPressPosition.y));
            zoom(lastPressPosition, lastPressPosition + sz);
        }
        isSelectionBoxActive = false;
    }
    else if (event.type == Event::MouseMoved) {
        if (event.mouseMove.y >= IMAGE_HEIGHT)
            isSelectionBoxActive = false;
    }
    else if (event.type == sf::Event::LostFocus) {
        isSelectionBoxActive = false;
    }
    else if (event.type == sf::Event::MouseWheelScrolled) {
        mouseWheelRotationPerFrame += event.mouseWheelScroll.delta;
    }
}

void drawBox() {
    Vector2i mousePos = Mouse::getPosition(window);

//    auto sz = makeSelectionBoxCorrect(Vector2f((mousePos.x - lastPressPosition.x),(mousePos.y - lastPressPosition.y)));
    auto sz = Vector2f((mousePos.x - lastPressPosition.x),(mousePos.y - lastPressPosition.y));

    RectangleShape rect(sz);
    rect.setFillColor(Color::Transparent);
    rect.setPosition(lastPressPosition.x, lastPressPosition.y);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(SELECTION_BOX_COLOR);
    window.draw(rect);
}

void saveImage() {
    std::string path = "mandelbrot" + std::to_string(time(0)) + ".png";
    mainRenderer.getMandelbrotSprite().getTexture()->copyToImage().saveToFile(path);
}

void createPanel() {
    auto* quitButton = new ImageButton("images/cross.png");
    quitButton->setPosition(WIDTH - 200, IMAGE_HEIGHT);
    quitButton->setSize(BUTTON_WIDTH, PANEL_HEIGHT);
    quitButton->setOnClick([] () {
        window.close();
    });
    gui.addWidget(quitButton);

    auto* revertButton = new ImageButton("images/return.png");
    revertButton->setPosition(0, IMAGE_HEIGHT);
    revertButton->setSize(BUTTON_WIDTH, PANEL_HEIGHT);
    revertButton->setOnClick([] () {
        revertFrame();
    });
    gui.addWidget(revertButton);

    auto* homeButton = new ImageButton("images/home.png");
    homeButton->setPosition(BUTTON_WIDTH + 20, IMAGE_HEIGHT);
    homeButton->setSize(BUTTON_WIDTH, PANEL_HEIGHT);
    homeButton->setOnClick([] () {
        currentFrame = START_FRAME;
        framesStack.push_back(currentFrame);
        mainRenderer.setFrame(currentFrame);
    });
    gui.addWidget(homeButton);

    auto* saveButton = new ImageButton("images/download.png");
    saveButton->setPosition(2 * BUTTON_WIDTH + 40, IMAGE_HEIGHT);
    saveButton->setSize(BUTTON_WIDTH, PANEL_HEIGHT);
    saveButton->setOnClick([] () {
        saveImage();
    });
    gui.addWidget(saveButton);

    themeSwitcherRenderer.create(2 * PANEL_HEIGHT, PANEL_HEIGHT, {-0.5, 0, 4, 2});
    themeSwitcherRenderer.setNumberOfIterations(50);
    auto* themeButton = new ThemeSwitcher(&themeSwitcherRenderer);
    themeButton->setPosition(3 * BUTTON_WIDTH + 80, IMAGE_HEIGHT);
    themeButton->setSize(2 * PANEL_HEIGHT, PANEL_HEIGHT);
    themeButton->setImage(themeSwitcherRenderer.getMandelbrotSprite());
    themeButton->setOnClick([] () {
        mainRenderer.setCurrentTheme(static_cast<ColoringTheme>((mainRenderer.getCurrentTheme() + 1) % NUMBER_OF_THEMES));
        themeSwitcherRenderer.setCurrentTheme(mainRenderer.getCurrentTheme());
    });
    gui.addWidget(themeButton);
}

void processMouseWheel() {
    if (mouseWheelRotationPerFrame != 0) {
        if (std::abs(mouseWheelRotationPerFrame) > 2) {
            mouseWheelRotationPerFrame = 2 * mouseWheelRotationPerFrame / std::abs(mouseWheelRotationPerFrame);
        }
        float k = std::max(1.0f, (float) mainRenderer.getNumberOfIterations() / MOUSE_WHEEL_SENSITIVITY);
        if (!mainRenderer.getIsBusy())
            mainRenderer.setNumberOfIterations(mainRenderer.getNumberOfIterations() + k * mouseWheelRotationPerFrame);
        mouseWheelRotationPerFrame = 0;
    }
}

int main() {
    if (!Shader::isAvailable()) {
        std::cerr << "Shaders is not available!" << std::endl;
        exit(0);
    }
    mainRenderer.create(WIDTH, IMAGE_HEIGHT, START_FRAME);

    window.create(VideoMode::getFullscreenModes()[0], "Mandelbrot Viewer", Style::Fullscreen);
    window.setFramerateLimit(15);
    createPanel();
    mainRenderer.updateImage();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (gui.handleEvent(event)) {
                isSelectionBoxActive = false;
                continue;
            };
            processEvent(event);
        }
        processMouseWheel();

        window.clear();
        window.draw(mainRenderer.getMandelbrotSprite());
        gui.draw();

        if (isSelectionBoxActive)
            drawBox();

        window.display();
    }

    return 0;
}