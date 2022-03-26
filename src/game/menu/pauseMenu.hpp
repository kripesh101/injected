#pragma once
#include <SFML/Graphics.hpp>
#include "button.hpp"

enum PauseMenuEvent {
    RESUME,
    RETURN_MAIN_MENU,
    QUIT
};

class PauseMenu {
private:
    sf::View view;

    sf::Font menuFont;
    
    sf::RectangleShape menuOverlay;
    Button resume;
    Button mainMenu;
    Button quit;

    sf::Texture logoTexture;
    sf::Sprite logoSprite;

    PauseMenuEvent response;
public:
    PauseMenu();

    const PauseMenuEvent& getResponse() const;
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    void onLoad(sf::RenderWindow& window);
};