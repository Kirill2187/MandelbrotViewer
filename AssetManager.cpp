#include "AssetManager.h"

AssetManager &AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

std::shared_ptr<sf::Texture> AssetManager::getTexture(const std::string &path) {
    auto it = textures.find(path);
    if (it == textures.end()) {
        std::shared_ptr<sf::Texture> txt = std::make_shared<sf::Texture>();
        txt->loadFromFile(path);
        textures[path] = txt;
        return txt;
    }
    return it->second;
}
