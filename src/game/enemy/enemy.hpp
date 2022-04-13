#pragma once

#include <SFML/Graphics.hpp>
#include <game/player.hpp>
#include <common/level.hpp>
#include <game/weapon/weapon.hpp>

class Enemy : public sf::Sprite {
private:
    // Within this radius, player is spotted even if not in visibility cone
    static constexpr float awarenessRadius = 40.f;

    // The angle of the visibility cone
    // static constexpr float visibilityAngle = 160.f;
    static constexpr float cosVA = 0.422618261741; // Corresponds to 130 degrees, cos(65)

    // Within this radius, enemy will shoot at player, if player is visible
    static constexpr float attackRadius = 175.f;

    // Within this radius, enemy will spot player, and get closer towards the attack radius if it can
    static constexpr float lineOfSightRadius = 600.f;

    // Position where player was last seen AND where the enemy can navigate to
    sf::Vector2f lastPlayerSeenNavigablePosition;

    float playerSeenCooldown;
    bool playerSeen;

    int health;
    float speed;

    sf::RectangleShape boundingBox;

    static sf::FloatRect calculateBounds(const sf::Vector2f& pos);

    Rifle gun;

public:
    Enemy();

    bool update(std::vector<Bullet>& bullets, const Level& level, const Player& player, const float& delta);
    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getGlobalBounds() const;
    const sf::RectangleShape& getBoundingBox() const;
    void bulletHit();
    bool isAlive() const;
};
