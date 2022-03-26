#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
private:
    sf::Text text;
    sf::RectangleShape container;
    bool activated = false;
    bool clicked = false;
public:
    Button(const sf::Font& font, const std::string& str, const float& posX, const float& posY, const int& size = 64);
    Button(const sf::Font& font, const std::string& str, const sf::Vector2f& pos, const int& size = 64);
    Button();

    void processInput(const sf::Vector2f& mousePos);
    bool isPressed() const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};