#pragma once

#include <SFML/Graphics.hpp>
#include "button.hpp"

enum class Agent {
    SHOTGUN,
    RIFLE
};

class CharacterSelect {
private:
    sf::View view;

    sf::Font menuFont;

    Button shotgun;
    sf::Texture shotgunTexture;
    sf::Sprite shotgunPortrait;
    
    Button rifle;
    sf::Texture rifleTexture;
    sf::Sprite riflePortrait;

    sf::Text heading;

    Agent response;
public:
    CharacterSelect();

    const Agent& getSelectedAgent() const;
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    // void onLoad(sf::RenderWindow& window);
};