#include "ImageButton.h"
#include "AssetManager.h"

bool ImageButton::mouseEntered() {
    return false;
}

bool ImageButton::mouseExit() {
    return false;
}

bool ImageButton::mouseClicked() {
    onClick();
    return true;
}

bool ImageButton::mousePressed() {
    return false;
}

void ImageButton::draw(sf::RenderTarget *target) {
    target->draw(!isMouseInside ? buttonUpSprite : (isMousePressedInside ? buttonDownSprite : buttonSelectedSprite));
    target->draw(image);
}

ImageButton::ImageButton(sf::String pathToTexture) {
    texture.loadFromFile(pathToTexture);
    image = sf::Sprite(texture);
    width = image.getTextureRect().width; height = image.getTextureRect().height;

    buttonDownSprite = sf::Sprite(*AssetManager::getInstance().getTexture("images/buttonDown.png"));
    buttonUpSprite = sf::Sprite(*AssetManager::getInstance().getTexture("images/buttonUp.png"));
    buttonSelectedSprite = sf::Sprite(*AssetManager::getInstance().getTexture("images/buttonSelected.png"));

    resizeSprite(buttonUpSprite, width, height);
    resizeSprite(buttonDownSprite, width, height);
    resizeSprite(buttonSelectedSprite, width, height);
}

void ImageButton::setPosition(int x, int y) {
    Widget::setPosition(x, y);
    buttonUpSprite.setPosition(x, y);
    buttonDownSprite.setPosition(x, y);
    buttonSelectedSprite.setPosition(x, y);
    updateImagePosition();
}

void ImageButton::setSize(int width, int height) {
    float scaleX = (float)width / this->width;
    float scaleY = (float)height / this->height;
    float imageScaleFactor = std::min(scaleX, scaleY);

    image.setScale(imageScaleFactor, imageScaleFactor);

    resizeSprite(buttonUpSprite, width, height);
    resizeSprite(buttonDownSprite, width, height);
    resizeSprite(buttonSelectedSprite, width, height);

    Widget::setSize(width, height);
    updateImagePosition();
}

void ImageButton::updateImagePosition() {
    image.setPosition(x + (width - image.getGlobalBounds().width) / 2,
                      y + (height - image.getGlobalBounds().height) / 2);
}

void ImageButton::resizeSprite(sf::Sprite &sprite, float width, float height) {
    sprite.setScale(sprite.getScale().x * width / sprite.getGlobalBounds().width,
                    sprite.getScale().y * height / sprite.getGlobalBounds().height);
}

void ImageButton::setOnClick(void (*func) ()) {
    onClick = func;
}
