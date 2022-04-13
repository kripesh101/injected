#pragma once
#include "player.hpp"
#include "enemy/enemy.hpp"
#include "weapon/bullet.hpp"
#include <common/level.hpp>
#include <SFML/Audio.hpp>
#include "menu/hud.hpp"

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

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    bool paused;
    bool ended;

    SimulationEvent currentEvent;
public:
    bool onLoad(const std::string& levelPath, sf::RenderWindow& window);
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    const SimulationEvent& getEvent() const;

    void setPaused(const bool& pause);
};
