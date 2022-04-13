#pragma once

#include <common/level.hpp>
#include "bullet.hpp"
#include <SFML/Audio.hpp>

class Weapon {
protected:
    sf::Sound sound;

    float timeBetweenShots;
    float timeSinceLastShot;
public:
    Weapon(const sf::SoundBuffer& soundBuffer, const float& fireRate);
    void update(const float& delta);
    virtual void shoot(const Level& level, std::vector<Bullet>& bullets, const sf::Sprite& origin, const bool& playerOwned) = 0;
};

class Rifle : public Weapon {
private:
    static sf::SoundBuffer soundBuffer;

public:
    Rifle();
    void shoot(const Level& level, std::vector<Bullet>& bullets, const sf::Sprite& origin, const bool& playerOwned);

    static bool loadSound();
};

class Shotgun : public Weapon {
private:
    static sf::SoundBuffer soundBuffer;

public:
    Shotgun();
    void shoot(const Level& level, std::vector<Bullet>& bullets, const sf::Sprite& origin, const bool& playerOwned);

    static bool loadSound();
};
