#include "button.hpp"
#include <iostream>

Button::Button(const sf::Font& font, const std::string& str, const float& posX, const float& posY, const int& size) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setString(str);
    text.setPosition(posX, posY);
}

Button::Button() {}

Button::Button(const sf::Font& font, const std::string& str, const sf::Vector2f& pos, const int& size) : Button(font, str, pos.x, pos.y, size) {}

void Button::processInput(const sf::Vector2f& mousePos) {
    if (text.getGlobalBounds().contains(mousePos)) {
        text.setStyle(sf::Text::Underlined);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            clicked = true;
        else {
            // If clicked in previous frame,
            // but mouse btn now unpressed currently
            // activate button
            if (clicked)
                activated = true;
            else
                activated = false;
            clicked = false;
        }

    } else {
        text.setStyle(sf::Text::Regular);
        activated = false;
        clicked = false;
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(text, states);
}

bool Button::isPressed() const {
    return activated;
}
