#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
private:
    sf::Text text;
    sf::RectangleShape container;
    bool activated = false;
    bool clicked = false;
    bool hovered = false;
    sf::Vector2f defaultPos;

    static sf::Cursor pointer;
    static sf::Cursor hand;
public:
    Button(const sf::Font& font, const std::string& str, const float& posX, const float& posY, const int& size = 64, const sf::Color& textColor = sf::Color::White);
    Button(const sf::Font& font, const std::string& str, const sf::Vector2f& pos, const int& size = 64, const sf::Color& textColor = sf::Color::White);
    Button();

    void processInput(sf::RenderWindow& window, const sf::Vector2f& mousePos);
    void setString(const sf::String& string);
    bool isPressed() const;
    static void loadCursor(const sf::RenderWindow& window);
    static void resetCursor(sf::RenderWindow& window);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};