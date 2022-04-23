#pragma once

#include <SFML/Graphics.hpp>
#include <game/player.hpp>
#include <common/level.hpp>
#include <game/weapon/weapon.hpp>
#include <memory>

class Enemy : public sf::Sprite {
private:
    // Within this radius, player is spotted even if not in visibility cone
    float awarenessRadius;

    // The angle of the visibility cone
    // static constexpr float visibilityAngle = 160.f;
    static constexpr float cosVA = 0.422618261741; // Corresponds to 130 degrees, cos(65)

    // Within this radius, enemy will shoot at player, if player is visible
    float attackRadius;

    float playerSeenCooldown;
    bool playerSeen;

    float hitFade;

    int health;
    float speed;

    sf::RectangleShape boundingBox;

    static sf::FloatRect calculateBounds(const sf::Vector2f& pos);

    // Weapon *gun;
    std::shared_ptr<Weapon> gun;

public:
    Enemy();
    // Enemy(const Enemy& obj);
    // ~Enemy();

    bool update(std::vector<Bullet>& bullets, const Level& level, const Player& player, const float& delta);
    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getGlobalBounds() const;
    const sf::RectangleShape& getBoundingBox() const;
    void bulletHit();
    bool isAlive() const;
};
