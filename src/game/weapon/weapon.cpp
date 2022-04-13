#include "weapon.hpp"
#include <cstdlib>

// #include <iostream>
// using std::cout;

void Weapon::update(const float& delta) {
    timeSinceLastShot += delta;
}

Weapon::Weapon(const sf::SoundBuffer& soundBuffer, const float& fireRate) :
    sound(soundBuffer),
    timeSinceLastShot(0.f),
    timeBetweenShots(60.f /fireRate)
{}

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
        int rotation = 9;
        while(rotation > 8) 
            rotation = 1 + std::rand()/((RAND_MAX + 1u)/8);

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
            int rotation = 19;
            while(rotation > 18) 
                rotation = 1 + std::rand()/((RAND_MAX + 1u)/18);
            
            int speed = 101;
            while (speed > 100)
                speed = 1 + std::rand()/((RAND_MAX + 1u)/100);

            Bullet bullet(playerOwned, origin.getPosition(), origin.getRotation() + (9.0f - rotation), 500.f - speed);
            if (bullet.initialCheck(level)) bullets.push_back(bullet);
        }
    }
}