#pragma once
#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "roundedRectangle.hpp"

enum class RestartMenuEvent {
    RESTART,
    RETURN_MAIN_MENU,
    QUIT
};

class RestartMenu {
private:
    sf::View view;

    sf::Font menuFont;
    
    sf::RoundedRectangleShape menuOverlay;
    Button restart;
    Button mainMenu;
    Button quit;

    sf::Text heading;

    RestartMenuEvent response;
public:
    RestartMenu();

    const RestartMenuEvent& getResponse() const;
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    // void onLoad(sf::RenderWindow& window);
};