#include "simulation.hpp"
#include <cmath>

// #include <iostream>
// using std::cout;

bool Simulation::onLoad(const std::string& levelPath, sf::RenderWindow& window) {
    if (!level.loadFromFolder(levelPath)) return false;
    player.setPosition(level.getPlayerSpawnPos());

    if (!gunSoundBuffer.loadFromFile("assets/sounds/ak47-1.wav")) return false;
    gunSound.setBuffer(gunSoundBuffer);

    if (!bulletTexture.loadFromFile("assets/textures/bullet_sprite.png")) return false;
    bulletTexture.setSmooth(false);

    if (!playerTexture.loadFromFile("assets/textures/player.png")) return false;
    playerTexture.setSmooth(false);

    if (!playerLegTexture.loadFromFile("assets/textures/legs.png")) return false;
    playerLegTexture.setSmooth(false);

    player.setTextures(playerTexture, playerLegTexture);

    return true;
}

bool Simulation::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {    
    if (window.hasFocus() && !paused) {
        player.processInput(window, mousePixelPos, level, deltaTime);
        
        constexpr float fireRate = 600.f; // bullets fired per minute
        constexpr float timeBetweenShots = 60.f / fireRate;

        static float timeSinceLastShot = 0.f;
        timeSinceLastShot += deltaTime;

        float bulletSpeed = 500.f * deltaTime;

        for (auto it = bullets.begin(); it != bullets.end(); ++it) {
            const float rotation = it->getRotation();
            const float radRotation = rotation * 3.1415f / 180.f;
            sf::Vector2f movement(cos(radRotation), sin(radRotation));
            movement *= bulletSpeed;

            for (int i = 0; i < 4; i++) {
                it->move(movement / 4.f);
                if (level.collides(it->getPosition(), true)) {
                    bullets.erase(it);
                    it--;
                    break;
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (timeSinceLastShot >= timeBetweenShots) {
                timeSinceLastShot = 0.f;

                gunSound.play();
                sf::Sprite bullet(bulletTexture);
                bullet.setOrigin(bullet.getLocalBounds().width / 2, bullet.getLocalBounds().height / 2);
                bullet.setPosition(player.getPosition());
                bullet.setRotation(player.getRotation());

                const float rotation = bullet.getRotation() * 3.1415f / 180.f;
                sf::Vector2f movement(cos(rotation), sin(rotation));
                bullet.move(movement * 20.f);

                if (!level.collides(bullet.getGlobalBounds(), true)) {
                    bullet.move(movement * 8.f);
                    bullets.push_back(bullet);
                }
            }
        }
    } else {
        // Reset view to player view if game paused
        // Menu rendering will change the view, so we must reset this
        window.setView(player.getView());
    }
        
    window.draw(level);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
        window.draw(player.getBoundingBox());
    }
    for (const auto& bullet : bullets)
        window.draw(bullet);
    player.drawLegs(window);
    window.draw(player);

    // Test level ending stuff - Press O to finish a level
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
        currentEvent = SimulationEvent::LEVEL_COMPLETE;
        ended = true;
        paused = true;
        return true;
    }

    // Test level ending stuff - Press L to finish a level, player die
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        currentEvent = SimulationEvent::PLAYER_DEAD;
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
