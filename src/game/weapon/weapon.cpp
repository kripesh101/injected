#include "weapon.hpp"
#include <cstdlib>
#include <common/utils.hpp>

// #include <iostream>
// using std::cout;

extern int volume;

void Weapon::update(const float& delta) {
    timeSinceLastShot += delta;
}

Weapon::Weapon(const sf::SoundBuffer& soundBuffer, const float& fireRate) :
    sound(soundBuffer),
    timeSinceLastShot(0.f),
    timeBetweenShots(60.f / fireRate)
{
    sound.setVolume(volume);
}

Weapon::~Weapon() {}

sf::SoundBuffer Rifle::soundBuffer = sf::SoundBuffer();
sf::SoundBuffer Shotgun::soundBuffer = sf::SoundBuffer();

bool Rifle::loadSound() {
    return soundBuffer.loadFromFile("assets/sounds/rifle.wav");
}

bool Shotgun::loadSound() {
    return soundBuffer.loadFromFile("assets/sounds/shotgun.wav");
}

Rifle::Rifle() : Weapon(soundBuffer, 500.f) {}
Shotgun::Shotgun() : Weapon(soundBuffer, 70.f) {}

void Rifle::shoot(const Level& level, std::vector<Bullet>& bullets, const sf::Sprite& origin, const bool& playerOwned) {
    if (timeSinceLastShot >= timeBetweenShots) {
        timeSinceLastShot = 0.f;

        sound.play();

        int rotation = rand(8);
        float bulletRotation = origin.getRotation();
        if (playerOwned) bulletRotation += 4.f - rotation;
        else bulletRotation += 8.f - rotation * 2;
        
        Bullet bullet(playerOwned, origin.getPosition(), bulletRotation);

        if (bullet.initialCheck(level)) bullets.push_back(bullet);
    }
}

void Shotgun::shoot(const Level& level, std::vector<Bullet>& bullets, const sf::Sprite& origin, const bool& playerOwned) {
    if (timeSinceLastShot >= timeBetweenShots) {
        timeSinceLastShot = 0.f;

        sound.play();
        for (int n=0; n != 6; ++n) {
            int rotation = rand(9, -9);
            int speed = rand(500, 400);

            Bullet bullet(playerOwned, origin.getPosition(), origin.getRotation() + rotation, speed);
            if (bullet.initialCheck(level)) bullets.push_back(bullet);
        }
    }
}