#include "ImageButton.h"
#include "iostream"

bool ImageButton::mouseEntered() {
    return false;
}

bool ImageButton::mouseExit() {
    return false;
}

bool ImageButton::mouseClicked() {
    return false;
}

void ImageButton::draw(sf::RenderTarget *target) {
    sf::RectangleShape rect({static_cast<float>(width), static_cast<float>(height)});
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Cyan);
    target->draw(rect);
    target->draw(image);
}

ImageButton::ImageButton(sf::String pathToTexture) {
    texture.loadFromFile(pathToTexture);
    image = sf::Sprite(texture);
    width = image.getTextureRect().width; height = image.getTextureRect().height;
}



void ImageButton::setPosition(int x, int y) {
    Widget::setPosition(x, y);
    updateImagePosition();
}

void ImageButton::setSize(int width, int height) {
    float imageScaleFactor = std::min((float)width / this->width, (float)height / this->height);
    image.setScale(imageScaleFactor, imageScaleFactor);
    Widget::setSize(width, height);
    updateImagePosition();
}

void ImageButton::updateImagePosition() {
    image.setPosition(x + (width - image.getGlobalBounds().width) / 2,
                      y + (height - image.getGlobalBounds().height) / 2);
}
