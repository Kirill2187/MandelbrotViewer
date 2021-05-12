#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "iostream"

using namespace sf;
using ld = long double;

#define PANEL_SIZE 0.06

enum ColoringTheme {
    RED, GREEN, BLUE, RAINBOW
};

struct Frame {
    ld cx, cy, sx, sy;
};

unsigned int width = VideoMode::getDesktopMode().width, height = VideoMode::getDesktopMode().height;
unsigned int panelHeight = static_cast<unsigned int>(height * PANEL_SIZE);
RenderWindow window;

bool isSelectionBoxActive = false;
Vector2i lastPressPosition;
const Color selectionBoxColor = Color(255, 50, 50);

Sprite mandelbrotImg;
RenderTexture mandelbrotTexture;

const Frame startFrame = {-0.5, 0, 2.3, 2.3 * 0.8};
Frame currentFrame = startFrame;
std::vector<Frame> framesStack = {startFrame};

std::map<std::string, ColoringTheme> themes = {
        {"Green", GREEN},
        {"Red", RED},
        {"Blue", BLUE},
        {"Rainbow", RAINBOW},
};
std::vector<int> iterations = {64, 128, 256, 512, 1024, 2048, 10000};

int max_iter = 256;
ColoringTheme currentTheme = GREEN;

void setMaxIter(int iter) {
    max_iter = iter;
}
int getMaxIter() {
    return max_iter;
}

void setTheme(ColoringTheme theme) {
    currentTheme = theme;
}
ColoringTheme getTheme() {
    return currentTheme;
}

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
    auto p = screenToWorld((static_cast<Vector2f>(p1) + static_cast<Vector2f>(p2)) / 2.0f);
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

int main() {
    if (!Shader::isAvailable()) {
        std::cout << "Shaders is not available!" << std::endl;
        exit(0);
    }

    mandelbrotShader.loadFromFile("mandelbrot.frag", Shader::Fragment);

    window.create(VideoMode::getFullscreenModes()[0], "Mandelbrot Viewer", Style::Fullscreen);
    window.setFramerateLimit(24);
    updateMandelbrot();

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