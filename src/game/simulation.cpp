#include "simulation.hpp"
#include <cmath>

// #include <iostream>
// using std::cout;

bool Simulation::onLoad(const std::string& levelPath, sf::RenderWindow& window) {
    if (!level.loadFromFolder(levelPath)) return false;
    player.setPosition(level.getPlayerSpawnPos());

    if (!Bullet::loadBulletTexture()) return false;

    if (!Rifle::loadSound()) return false;
    if (!Shotgun::loadSound()) return false;

    if (agent == Agent::SHOTGUN) {
        if (!playerTexture.loadFromFile("assets/textures/player_shotgun.png")) return false;
    } else if (agent == Agent::RIFLE) {
        if (!playerTexture.loadFromFile("assets/textures/player_rifle.png")) return false;
    }
    playerTexture.setSmooth(false);

    if (!playerLegTexture.loadFromFile("assets/textures/legs.png")) return false;
    playerLegTexture.setSmooth(false);

    player.setTextures(playerTexture, playerLegTexture);

    if (!enemyTexture.loadFromFile("assets/textures/enemy.png")) return false;
    enemyTexture.setSmooth(false);

    for (const auto& eData : level.getEnemyData()) {
        Enemy enemy;
        enemy.setTexture(enemyTexture, true);
        enemy.setPosition(eData.position);
        enemy.setRotation(eData.rotation);
        enemies.push_back(enemy);
    }

    return true;
}

bool Simulation::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {    
    extern bool DEBUG;
    
    if (window.hasFocus() && !paused) {
        for (auto it = bullets.begin(); it != bullets.end(); ++it) {
            if (it->update(level, enemies, player, deltaTime)) {
                bullets.erase(it);
                it--;
            }
        }

        player.processInput(window, mousePixelPos, bullets, level, deltaTime);

        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (it->isAlive()) {
                it->update(bullets, level, player, deltaTime);
            } else {
                enemies.erase(it);
                it--;
            }
        }
    } else {
        // Reset view to player view if game paused
        // Menu rendering will change the view, so we must reset this
        window.setView(player.getView());
    }
        
    window.draw(level);
    if (DEBUG && sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
        window.draw(player.getBoundingBox());

        for (const auto& enemy : enemies)
            window.draw(enemy.getBoundingBox());
    }
    for (const auto& bullet : bullets)
        window.draw(bullet);
    for (const auto& enemy : enemies)
        window.draw(enemy);

    hud.update(window, player);
    if (player.isAlive()) {
        player.drawLegs(window);
        window.draw(player);
    } else {
        currentEvent = SimulationEvent::PLAYER_DEAD;
        ended = true;
        paused = true;
        return true;
    }

    if (DEBUG) {
        // Test level ending stuff - Press O to finish a level
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            enemies.clear();
        }

        // Test level ending stuff - Press L to finish a level, player die
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            currentEvent = SimulationEvent::PLAYER_DEAD;
            ended = true;
            paused = true;
            return true;
        }
    }   

    if (enemies.empty()) {
        currentEvent = SimulationEvent::LEVEL_COMPLETE;
        ended = true;
        paused = true;
        return true;
    }

    return false;
}

void Simulation::setPaused(const bool& pause) {
    if (!ended)
        paused = pause;
}

const SimulationEvent& Simulation::getEvent() const {
    return currentEvent;
}

Simulation::Simulation(const Agent& agent) :
    agent(agent)
{
    if (agent == Agent::RIFLE)
        player.setGunType(GunType::RIFLE);
    else if (agent == Agent::SHOTGUN)
        player.setGunType(GunType::SHOTGUN);
}
