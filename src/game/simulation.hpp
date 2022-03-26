#pragma once
#include "player.hpp"
#include <common/level.hpp>
#include <SFML/Audio.hpp>

class Simulation {
private:
    Level level;
    Player player;

    sf::Sound gunSound;
    sf::SoundBuffer gunSoundBuffer;
    sf::Texture bulletTexture;
    sf::Texture playerTexture;

    std::vector<sf::Sprite> bullets;

    bool paused = false;
public:
    bool onLoad(const std::string& levelPath, sf::RenderWindow& window);
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);

    void setPaused(const bool& pause);
};
