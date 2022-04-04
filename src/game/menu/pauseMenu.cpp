#include <SFML/Graphics.hpp>
#include <common/utils.hpp>
#include "pauseMenu.hpp"

bool PauseMenu::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {   
    window.setView(view);

    if (window.hasFocus()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        resume.processInput(window, mousePos);
        mainMenu.processInput(window, mousePos);
        quit.processInput(window, mousePos);
    }


    window.draw(menuOverlay);
    window.draw(logoSprite);
    
    window.draw(resume);
    window.draw(mainMenu);    
    window.draw(quit);

    if (resume.isPressed()) {
        response = RESUME;
        return true;
    }
    
    if (mainMenu.isPressed()) {
        response = RETURN_MAIN_MENU;
        return true;
    }

    if (quit.isPressed()) {
        response = QUIT;
        return true;
    }

    window.display();

    return false;
}

const PauseMenuEvent& PauseMenu::getResponse() const {
    return response;
}

PauseMenu::PauseMenu() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f , 1080.f)),
    menuOverlay(sf::Vector2f(600.f, 600.f), 20.f, 10),
    
    // Buttons
    resume  (menuFont, "RESUME",    740.f, 500.f, 48),
    mainMenu(menuFont, "MAIN MENU", 740.f, 620.f, 48),
    quit    (menuFont, "QUIT",      740.f, 740.f, 48)
{
    menuOverlay.setPosition(660.f, 240.f);
    menuOverlay.setFillColor(sf::Color::Black);

    menuFont.loadFromFile("assets/fonts/main_menu.ttf");
    
    logoTexture.loadFromFile("assets/textures/logo.png");
    logoSprite.setTexture(logoTexture, true);
    logoSprite.setScale(0.40f, 0.40f);
    logoSprite.setPosition(730.f, 280.f);
}

void PauseMenu::onLoad(sf::RenderWindow& window) {
    window.setTitle("INJECTED! - PAUSED");
}
