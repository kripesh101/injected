#include "simulation.hpp"
#include <cmath>

#include <iostream>
using std::cout;

bool Simulation::onLoad(const std::string& levelPath, sf::RenderWindow& window) {
    if (!level.loadFromFolder(levelPath)) return false;
    player.setPosition(level.getPlayerSpawnPos());

    if (!gunSoundBuffer.loadFromFile("assets/sounds/ak47-1.wav")) return false;
    gunSound.setBuffer(gunSoundBuffer);

    if (!bulletTexture.loadFromFile("assets/textures/bullet_sprite.png")) return false;
    bulletTexture.setSmooth(false);

    if (!playerTexture.loadFromFile("assets/textures/player.png")) return false;
    playerTexture.setSmooth(false);
    player.setTexture(playerTexture, true);

    return true;
}

bool Simulation::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {    
    window.setView(player.getView());

    if (window.hasFocus() && !paused) {
        constexpr float fireRate = 600.f; // bullets fired per minute
        constexpr float timeBetweenShots = 60.f / fireRate;

        static float timeSinceLastShot = 0.f;
        timeSinceLastShot += deltaTime;

        float bulletSpeed = 500.f * deltaTime;

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
                bullet.move(movement * 16.f);

                if (!level.collides(bullet.getGlobalBounds(), true)) {
                    bullet.move(movement * 8.f);
                    bullets.push_back(bullet);
                }
            }
        }

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

        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        player.processInput(mousePos, level, deltaTime);
    }
        
    window.draw(level);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
        window.draw(player.getBoundingBox());
    }
    for (const auto& bullet : bullets)
        window.draw(bullet);
    window.draw(player);

    return false;
}

void Simulation::setPaused(const bool& pause) {
    paused = pause;
}
