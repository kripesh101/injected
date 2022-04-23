#include "characterSelect.hpp"

CharacterSelect::CharacterSelect() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f , 1080.f)),
    
    // Buttons
    shotgun (menuFont, "SHOTGUN HARRY",   210.f, 820.f),
    rifle   (menuFont, "RIFLE LARRY", 1340.f, 820.f),

    heading ("CHOOSE YOUR AGENT", menuFont, 64)
{
    heading.setPosition(650.f, 100.f);
    menuFont.loadFromFile("assets/fonts/main_menu.ttf");

    rifleTexture.loadFromFile("assets/textures/agents/rifle.png");
    shotgunTexture.loadFromFile("assets/textures/agents/shotgun.png");

    shotgunPortrait.setTexture(shotgunTexture, true);
    shotgunPortrait.setPosition(230.f, 450.f);
    
    riflePortrait.setTexture(rifleTexture, true);
    riflePortrait.setPosition(1340.f, 450.f);
}

const Agent& CharacterSelect::getSelectedAgent() const {
    return response;
}

bool CharacterSelect::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {
    window.setView(view);

    if (window.hasFocus()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        shotgun.processInput(window, mousePos);
        rifle.processInput(window, mousePos);
    }

    window.draw(shotgun);
    window.draw(shotgunPortrait);

    window.draw(rifle);
    window.draw(riflePortrait);

    window.draw(heading);

    if (shotgun.isPressed()) {
        response = Agent::SHOTGUN;
        return true;
    }

    if (rifle.isPressed()) {
        response = Agent::RIFLE;
        return true;
    }

    return false;
}
