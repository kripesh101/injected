#include "bullet.hpp"
#include <cmath>
#include <game/player.hpp>
#include <game/enemy/enemy.hpp>

// #include <iostream>
// using std::cout;

Bullet::Bullet(bool playerOwned, sf::Vector2f position, float rotation) :
    Bullet(playerOwned, position, rotation, 500.f)
{}

Bullet::Bullet(bool playerOwned, sf::Vector2f position, float rotation, float speed) :
    decayTime(10.f),
    speed(speed),
    playerOwned(playerOwned)
{
    setPosition(position);
    setRotation(rotation);

    direction = sf::Vector2f(cos(rotation * 3.1415f / 180.f), sin(rotation * 3.1415f / 180.f));

    setTexture(bulletTexture, true);
    setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
}

sf::Texture Bullet::bulletTexture = sf::Texture();

void Bullet::moveInDirection(const float& multiplier) {
    move(direction * multiplier);
}

bool Bullet::loadBulletTexture() {
    if (!bulletTexture.loadFromFile("assets/textures/bullet_sprite.png")) return false;
    bulletTexture.setSmooth(false);
    return true;
}

bool Bullet::initialCheck(const Level& level) {
    moveInDirection(20.f);

    if (!level.collides(getGlobalBounds(), true)) {
        moveInDirection(8.f);
        return true;
    }
    return false;
}


bool Bullet::update(const Level& level, std::vector<Enemy>& enemies, Player& player, const float& delta) {
    sf::Vector2f movement = direction * speed * delta;

    for (int i = 0; i < 8; i++) {
        move(movement / 8.f);

        // cout << playerOwned << " " << getPosition().x << " " << getPosition().y << "\n";
        if (!playerOwned && player.getGlobalBounds().contains(getPosition())) {
            player.bulletHit();
            return true;
        }

        for (auto &enemy : enemies) {
            if (enemy.getGlobalBounds().contains(getPosition())) {
                enemy.bulletHit();
                return true;
            }
        }
        
        if (level.collides(getPosition(), true)) return true;
    }

    // Despawn bullet
    decayTime -= delta;
    if (decayTime <= 0.f) return true;

    return false;
}

const bool& Bullet::isPlayedOwned() const {
    return playerOwned;
}