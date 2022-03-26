#include <SFML/Graphics.hpp>
#include <common/utils.hpp>
#include "mainMenu.hpp"

bool MainMenu::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {   
    window.setView(view);
    
    if (window.hasFocus()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        newGame.processInput(mousePos);
        quit.processInput(mousePos);
    }

    window.draw(logoSprite);
    window.draw(newGame);
    window.draw(quit);

    if (newGame.isPressed()) {
        response.type = response.NEW_GAME;
        response.levelName = "assets/editor_default_level/";
        return true;
    }
    
    if (quit.isPressed()) {
        response.type = response.QUIT;
        return true;
    }

    window.display();

    return false;
}

MainMenu::MainMenu() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f, 1080.f)),
    
    // Buttons
    newGame (menuFont, "NEW GAME",  100.f, 600.f),
    quit    (menuFont, "QUIT",      100.f, 750.f)
{
    menuFont.loadFromFile("assets/fonts/main_menu.ttf");
    logoTexture.loadFromFile("assets/textures/logo.png");
    logoSprite.setTexture(logoTexture, true);
    logoSprite.setPosition(50.f, 30.f);
}

const MainMenuEvent& MainMenu::getResponse() const {
    return response;
}

void MainMenu::onLoad(sf::RenderWindow& window) {
    window.setTitle("INJECTED! - MAIN MENU");
}
