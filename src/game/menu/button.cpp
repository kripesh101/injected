#include "button.hpp"
// #include <iostream>

Button::Button(const sf::Font& font, const std::string& str, const float& posX, const float& posY, const int& size, const sf::Color& textColor) : defaultPos(posX, posY) {
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(textColor);
    text.setString(str);
    text.setPosition(defaultPos);
}

Button::Button() {}

sf::Cursor Button::pointer = sf::Cursor();
sf::Cursor Button::hand = sf::Cursor();

void Button::resetCursor(sf::RenderWindow& window) {
    window.setMouseCursor(pointer);
}

void Button::loadCursor(const sf::RenderWindow& window) {
    pointer.loadFromSystem(sf::Cursor::Arrow);
    hand.loadFromSystem(sf::Cursor::Hand);
}


Button::Button(const sf::Font& font, const std::string& str, const sf::Vector2f& pos, const int& size, const sf::Color& textColor) : Button(font, str, pos.x, pos.y, size, textColor) {}

void Button::setString(const sf::String& string) {
    text.setString(string);
}

void Button::processInput(sf::RenderWindow& window, const sf::Vector2f& mousePos) {
    // Restore to default position for collision checking
    text.setPosition(defaultPos);
    
    if (text.getGlobalBounds().contains(mousePos)) {
        if (!hovered) {
            hovered = true;
            window.setMouseCursor(hand);
        }
        text.setStyle(sf::Text::Underlined);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            clicked = true;
            activated = false;
        } else {
            // If clicked in previous frame,
            // but mouse btn now unpressed currently
            // activate button
            if (clicked) {
                activated = true;
                window.setMouseCursor(pointer);
                hovered = false;
            } else {
                activated = false;
            }
            clicked = false;
        }

    } else {
        if (hovered) {
            hovered = false;
            window.setMouseCursor(pointer);
        }

        text.setStyle(sf::Text::Regular);
        activated = false;
        clicked = false;
    }

    if (clicked)
        text.setPosition(defaultPos + sf::Vector2f(0, 5));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(text, states);
}

bool Button::isPressed() const {
    return activated;
}
