#pragma once

#include <SFML/Graphics.hpp>
#include <common/level.hpp>
#include <vector>

class Player;
class Enemy;

class Bullet : public sf::Sprite {
private:
    float decayTime;
    float speed;
    sf::Vector2f direction;
    bool playerOwned;
    

    static sf::Texture bulletTexture;
public:
    Bullet(bool playerOwned, sf::Vector2f position, float rotation, float speed);
    Bullet(bool playerOwned, sf::Vector2f position, float rotation);

    static bool loadBulletTexture();
    void moveInDirection(const float& multiplier);
    bool initialCheck(const Level& level);
    bool update(const Level& level, std::vector<Enemy>& enemies, Player& player, const float& delta);
    const bool& isPlayedOwned() const;
};