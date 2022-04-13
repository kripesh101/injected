#pragma once
#include <SFML/Graphics.hpp>
#include <game/player.hpp>
#include "roundedRectangle.hpp"

class HUD {
private:
    sf::View view;

    sf::Texture heartTexture;
    sf::Sprite heart1;
    sf::Sprite heart2;
    sf::Sprite heart3;

    sf::Font font;
    sf::Text healthText;

    sf::RoundedRectangleShape overlay;
public:
    HUD();

    void update(sf::RenderWindow& window, const Player& player);
};