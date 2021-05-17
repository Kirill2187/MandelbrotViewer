#ifndef MANDELBROTVIEWER_ASSETMANAGER_H
#define MANDELBROTVIEWER_ASSETMANAGER_H

#include "memory"
#include "iostream"
#include "SFML/Graphics.hpp"

class AssetManager {
public:
    static AssetManager& getInstance();
    std::shared_ptr<sf::Texture> getTexture(const std::string& path);

private:
    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    AssetManager() = default;
};


#endif //MANDELBROTVIEWER_ASSETMANAGER_H
