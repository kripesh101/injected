#pragma once
#include <SFML/Graphics.hpp>
#include "button.hpp"

struct MainMenuEvent {
    enum {
        QUIT,
        NEW_GAME
    } type;
    std::string levelName;
};

class MainMenu {
private:
    sf::View view;

    sf::Font menuFont;
    Button newGame;
    Button quit;

    sf::Texture logoTexture;
    sf::Sprite logoSprite;
    MainMenuEvent response;
public:
    MainMenu();

    const MainMenuEvent& getResponse() const;
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    void onLoad(sf::RenderWindow& window);
};