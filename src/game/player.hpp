#pragma once
#include <SFML/Graphics.hpp>
#include <common/level.hpp>

class Player : public sf::Sprite {
private:
    sf::View view;

    sf::RectangleShape boundingBox;
    
    int health;
    float speed = 100.f;

    static sf::FloatRect calculateBounds(const sf::Vector2f& pos);
public:
    Player();

    void processInput(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const Level& level, const float& delta);
    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getGlobalBounds() const;
    const sf::RectangleShape& getBoundingBox() const;
    const sf::View& getView() const;
};
