#include "hud.hpp"

HUD::HUD() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f , 1080.f)),
    healthText("HEALTH", font, 32),
    overlay(sf::Vector2f(180.f, 130.f), 20.f, 10)
{
    overlay.setPosition(30.f, 10.f);
    overlay.setFillColor(sf::Color::Black);

    font.loadFromFile("assets/fonts/hud.ttf");
    healthText.setPosition(50.f, 20.f);

    heartTexture.loadFromFile("assets/textures/heart.png");
    heart1.setTexture(heartTexture, true);
    heart2.setTexture(heartTexture, true);
    heart3.setTexture(heartTexture, true);

    heart1.setPosition(50.f, 80.f);
    heart2.setPosition(100.f, 80.f);
    heart3.setPosition(150.f, 80.f);
}

void HUD::update(sf::RenderWindow& window, const Player& player) {
    window.setView(view);

    window.draw(overlay);

    window.draw(healthText);

    const int& health = player.getHealth();
    heart1.setColor(sf::Color::White);
    heart2.setColor(sf::Color::White);
    heart3.setColor(sf::Color::White);
    
    const sf::Color dimmed = sf::Color(64, 64, 64);
    if (health <= 2) heart3.setColor(dimmed);
    if (health <= 1) heart2.setColor(dimmed);
    if (health <= 0) heart1.setColor(dimmed);

    window.draw(heart1);
    window.draw(heart2);
    window.draw(heart3);
}
