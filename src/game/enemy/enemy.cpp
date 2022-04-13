#include "enemy.hpp"
#include <cmath>

// #include <iostream>
// using std::cout;

Enemy::Enemy() : 
    boundingBox(sf::Vector2f(16.f, 16.f)),
    health(2),
    speed(100.f),
    playerSeenCooldown(0.65f),
    playerSeen(false)
{
    setOrigin(24.f, 24.f);   
}

void Enemy::setPosition(const sf::Vector2f& pos) {
    sf::Sprite::setPosition(pos);
    const sf::Vector2f topLeft = getPosition() - sf::Vector2f(8.f, 8.f);
    boundingBox.setPosition(topLeft);
}

bool Enemy::update(std::vector<Bullet>& bullets, const Level& level, const Player& player, const float& delta) {
    gun.update(delta);
    
    const sf::Vector2f enemyToPlayer = player.getPosition() - getPosition();
    const float distance = std::sqrt(enemyToPlayer.x * enemyToPlayer.x + enemyToPlayer.y * enemyToPlayer.y);
    if (distance <= attackRadius) {
        const float angleToPlayer = std::atan2(enemyToPlayer.y, enemyToPlayer.x)  * 180.f / 3.1415f;
        const float facingAngle = getRotation() * 3.1415f / 180.f;

        const float cosAngle = (cos(facingAngle) * (enemyToPlayer.x) + sin(facingAngle) * (enemyToPlayer.y)) / distance;
        
        if (
            (distance <= awarenessRadius || cosAngle >= cosVA) &&
            !level.lineCollides(getPosition(), player.getPosition(), true)
        ) {
            setRotation(angleToPlayer);
            playerSeenCooldown -= delta;
            if (playerSeenCooldown <= 0.f) {
                playerSeen = true;
                gun.shoot(level, bullets, *this, false);
                return true;
            }
        }
    } else {
        if (playerSeen) playerSeenCooldown = 0.25f;
    }
    return false;
}


sf::FloatRect Enemy::calculateBounds(const sf::Vector2f& pos) {
    const sf::Vector2f topLeft = pos - sf::Vector2f(8.f, 8.f);
    
    return sf::FloatRect(
        topLeft,
        sf::Vector2f(16.f, 16.f)
    );
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return calculateBounds(getPosition());
}

const sf::RectangleShape& Enemy::getBoundingBox() const {
    return boundingBox;
}

void Enemy::bulletHit() {
    health--;
}

bool Enemy::isAlive() const {
    return (health > 0);
}
