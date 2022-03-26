#include <SFML/Graphics.hpp>

#include <common/level.hpp>
#include <common/utils.hpp>

// #include <iostream>
// using std::cout;

#include "menu/mainMenu.hpp"
#include "menu/pauseMenu.hpp"
#include "simulation.hpp"

enum State {
    MAIN_MENU,
    IN_GAME,
    PAUSED
};
State currentState = MAIN_MENU;

int gameMain()
{
    // Game Window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED!", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    // window.setMouseCursorGrabbed(true);

    MainMenu mainMenu;
    PauseMenu pauseMenu;
    mainMenu.onLoad(window);

    Simulation sim;

    sf::Clock clock;

    // Game Loop
    while (window.isOpen())
    {
        float delta = clock.restart().asSeconds();

        const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (currentState == IN_GAME) {
                        currentState = PAUSED;
                        sim.setPaused(true);
                        pauseMenu.onLoad(window);
                    }
                    else if (currentState == PAUSED) {
                        currentState = IN_GAME;
                        sim.setPaused(false);
                        window.setTitle("INJECTED!");
                    }
                }
            }
        }

        window.clear(smoothColor(delta));

        if (currentState == MAIN_MENU) {
            if (mainMenu.update(window, mousePixelPos, delta)) {
                auto response = mainMenu.getResponse();
                switch (response.type) {
                case response.QUIT:
                    window.close();
                    break;
                case response.NEW_GAME:
                    sim = Simulation();
                    if (!sim.onLoad(response.levelName, window)) return -1;
                    currentState = IN_GAME;
                    window.setTitle("INJECTED!");
                }
            }
            continue;
        }

        sim.update(window, mousePixelPos, delta);

        if (currentState == PAUSED) {
            // Draw and process pause UI
            if (pauseMenu.update(window, mousePixelPos, delta)) {
                auto response = pauseMenu.getResponse();
                switch (response) {
                case RESUME:
                    currentState = IN_GAME;
                    sim.setPaused(false);
                    window.setTitle("INJECTED!");
                    break;
                case RETURN_MAIN_MENU:
                    currentState = MAIN_MENU;
                    mainMenu.onLoad(window);
                    break;
                case QUIT:
                    window.close();
                }
            }
            continue;
        }

        window.display();
    }

    return 0;
}
