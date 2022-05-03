#pragma once
#include "player.hpp"
#include "enemy/enemy.hpp"
#include "weapon/bullet.hpp"
#include <common/level.hpp>
#include <SFML/Audio.hpp>
#include "menu/hud.hpp"
#include "menu/characterSelect.hpp"

enum class SimulationEvent {
    PLAYER_DEAD,
    LEVEL_COMPLETE
};

class Simulation {
private:
    Level level;
    Player player;

    HUD hud;

    sf::Texture playerTexture;
    sf::Texture playerLegTexture;
    sf::Texture enemyTexture;
    sf::Texture heartTexture;

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<sf::Sprite> hearts;

    bool paused = false;
    bool ended = false;

    Agent agent;

    SimulationEvent currentEvent;
public:
    Simulation(const Agent& agent);

    bool onLoad(const std::string& levelPath, sf::RenderWindow& window);
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    const SimulationEvent& getEvent() const;

    void setPaused(const bool& pause);
};
