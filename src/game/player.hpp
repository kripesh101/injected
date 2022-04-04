#pragma once
#include <SFML/Graphics.hpp>
#include <common/level.hpp>
#include "animation/AnimatedSprite.hpp"

class Player : public sf::Sprite {
private:
    sf::View view;

    sf::RectangleShape boundingBox;
    
    int health;
    float speed;

    static sf::FloatRect calculateBounds(const sf::Vector2f& pos);

    AnimatedSprite legs;
    Animation legAnimation;
    sf::Texture legSpriteSheet;

public:
    Player();

    void processInput(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const Level& level, const float& delta);
    void setPosition(const sf::Vector2f& pos);
    void setTextures(const sf::Texture& player, const sf::Texture& legTex);
    sf::FloatRect getGlobalBounds() const;
    const sf::RectangleShape& getBoundingBox() const;
    const sf::View& getView() const;
    void drawLegs(sf::RenderWindow& window) const;
};
