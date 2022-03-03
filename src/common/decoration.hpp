#pragma once
#include <SFML/Graphics.hpp>

struct Decoration : public sf::Sprite {
    bool collidable;
    std::string texLocation;

    bool collidesWith(sf::FloatRect bb) const;
};

struct FileTexture : public sf::Texture {
    std::string location;
};

bool deserializeDecorations(const std::string& filePath, std::vector<Decoration>& decorations, std::vector<FileTexture>& textures);
void serializeDecorations(const std::string& filePath, std::vector<Decoration>& array);
bool decorCollides(const sf::FloatRect& bb, const std::vector<Decoration>& array);