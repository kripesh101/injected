#pragma once
#include "player.hpp"
#include <common/level.hpp>
#include <SFML/Audio.hpp>

enum class SimulationEvent {
    PLAYER_DEAD,
    LEVEL_COMPLETE
};

class Simulation {
private:
    Level level;
    Player player;

    sf::Sound gunSound;
    sf::SoundBuffer gunSoundBuffer;
    sf::Texture bulletTexture;
    sf::Texture playerTexture;
    sf::Texture playerLegTexture;

    std::vector<sf::Sprite> bullets;

    bool paused = false;
    bool ended = false;

    SimulationEvent currentEvent;
public:
    bool onLoad(const std::string& levelPath, sf::RenderWindow& window);
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    const SimulationEvent& getEvent() const;

    void setPaused(const bool& pause);
};
