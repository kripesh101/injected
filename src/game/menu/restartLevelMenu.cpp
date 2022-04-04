#include "restartLevelMenu.hpp"

RestartMenu::RestartMenu() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f , 1080.f)),
    menuOverlay(sf::Vector2f(600.f, 600.f), 20.f, 10),
    
    // Buttons
    restart (menuFont, "RESTART",   740.f, 500.f, 48),
    mainMenu(menuFont, "MAIN MENU", 740.f, 620.f, 48),
    quit    (menuFont, "QUIT",      740.f, 740.f, 48),
    
    // Heading Text
    heading ("YOU   DIED", menuFont, 96)
{
    heading.setFillColor(sf::Color::Red);
    heading.setPosition(740.f, 280.f);
    menuOverlay.setPosition(660.f, 240.f);
    menuOverlay.setFillColor(sf::Color::Black);

    menuFont.loadFromFile("assets/fonts/main_menu.ttf");
}

const RestartMenuEvent& RestartMenu::getResponse() const {
    return response;
}

bool RestartMenu::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {   
    window.setView(view);

    if (window.hasFocus()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        restart.processInput(window, mousePos);
        mainMenu.processInput(window, mousePos);
        quit.processInput(window, mousePos);
    }


    window.draw(menuOverlay);
    window.draw(heading);
    
    window.draw(restart);
    window.draw(mainMenu);    
    window.draw(quit);

    if (restart.isPressed()) {
        response = RestartMenuEvent::RESTART;
        return true;
    }
    
    if (mainMenu.isPressed()) {
        response = RestartMenuEvent::RETURN_MAIN_MENU;
        return true;
    }

    if (quit.isPressed()) {
        response = RestartMenuEvent::QUIT;
        return true;
    }

    return false;
}