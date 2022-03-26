#pragma once
#include <SFML/Graphics.hpp>

struct Decoration : public sf::Sprite {
    bool collidable;
    std::string texLocation;

    bool collidesWith(const sf::FloatRect& bb) const;
    bool collidesWith(const sf::Vector2f& point) const;
};

struct FileTexture : public sf::Texture {
    std::string location;
};

bool deserializeDecorations(const std::string& filePath, std::vector<Decoration>& decorations, std::vector<FileTexture>& textures);
void serializeDecorations(const std::string& filePath, const std::vector<Decoration>& decorations);

bool decorCollides(const sf::FloatRect& bb, const std::vector<Decoration>& decorations);
bool decorCollides(const sf::Vector2f& point, const std::vector<Decoration>& decorations);
