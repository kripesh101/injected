#include "mainMenu.hpp"
#include <filesystem.hpp>
#include <fstream>
#include <common/utils.hpp>
namespace fs = ghc::filesystem;

// #include <iostream>
// using std::cout;

int volume = 50;

bool MainMenu::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {   
    window.setView(view);
    
    if (window.hasFocus()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        if (!missionView) {
            newGame.processInput(window, mousePos);
            quit.processInput(window, mousePos);
            volumeUp.processInput(window, mousePos);
            volumeDown.processInput(window, mousePos);
            fullscreenToggle.processInput(window, mousePos);
        } else {
            missionLeft.processInput(window, mousePos);
            missionRight.processInput(window, mousePos);
            missionBack.processInput(window, mousePos);
            missionPlay.processInput(window, mousePos);
        }
    }

    volumeLabel.setString("      " + std::to_string(volume) + "%\nGame Volume");

    window.draw(logoSprite);
    window.draw(newGame);
    window.draw(quit);
    window.draw(volumeUp);
    window.draw(volumeLabel);
    window.draw(volumeDown);
    window.draw(fullscreenToggle);


    if (missionView) {
        window.draw(missionMenuOverlay);
        window.draw(missionLeft);
        window.draw(missionRight);
        window.draw(missionBack);
        window.draw(missionPlay);
        window.draw(missionDescription);
        window.draw(missionTitle);
        window.draw(missionThumbnail);

        if (missionLeft.isPressed()) updateMissionView(currentMission - 1);

        if (missionRight.isPressed()) updateMissionView(currentMission + 1);

        if (missionBack.isPressed()) missionView = false;

        if (missionPlay.isPressed()) {
            missionView = false;
            response.type = response.NEW_GAME;
            response.missionPath = missionPaths[currentMission];
            return true;
        }

    } else {
        if (fullscreenToggle.isPressed()) {
            static bool fullscreen = false;
            
            sf::ContextSettings settings;
            settings.antialiasingLevel = 0;
            
            window.close();
            
            if (fullscreen)
                window.create(sf::VideoMode(1200, 640), "INJECTED!", sf::Style::Default, settings);
            else
                window.create(sf::VideoMode::getDesktopMode(), "INJECTED!", sf::Style::None, settings);
            
            window.setVerticalSyncEnabled(true);
            fullscreen = !fullscreen;

            if (fullscreen) fullscreenToggle.setString("Fullscreen ON");
            else fullscreenToggle.setString("Fullscreen OFF");
        }

        if (volumeUp.isPressed()) {
            if (volume < 10) volume += 2;
            else volume += 10;
            if (volume > 100) volume = 100;
        }

        if (volumeDown.isPressed()) {
            if (volume <= 10) volume -= 2;
            else volume -= 10;
            if (volume < 0) volume = 0;
        }

        if (newGame.isPressed()) {

            missionPaths.clear();
            for (const auto & dir : fs::directory_iterator("assets/missions")) {
                if (dir.is_directory()) {
                    if (fs::exists(dir.path().string() + "/mission_details.txt")) {
                        missionPaths.push_back(dir.path().string() + "/");
                    }
                }
            }

            if (missionPaths.size() == 0) {
                response.type = response.QUIT;
                return true;
            }

            if (missionPaths.size() == 1) {
                response.type = response.NEW_GAME;
                response.missionPath = missionPaths[0];
                return true;
            }

            missionView = true;
            updateMissionView(0);
        }
        
        if (quit.isPressed()) {
            response.type = response.QUIT;
            return true;
        }
    }

    window.display();

    return false;
}

MainMenu::MainMenu() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f, 1080.f)),
    missionMenuOverlay(sf::Vector2f(900.f, 700.f), 20.f, 10),
    
    // Buttons
    newGame         (menuFont, "NEW GAME",  100.f, 600.f),
    quit            (menuFont, "QUIT",      100.f, 750.f),
    missionLeft     (menuFont, "<",         550.f, 340.f),
    missionRight    (menuFont, ">",         1320.f, 340.f),
    missionBack     (menuFont, "BACK",      550.f, 220.f, 40),
    missionPlay     (menuFont, "PLAY",      880.f, 800.f, 72),
    volumeUp        (menuFont, "+",         1330.f, 950.f),
    volumeDown      (menuFont, "-",         1850.f, 950.f),
    fullscreenToggle(menuFont, "Fullscreen OFF", 1370.f, 770.f),

    // Text
    missionTitle      ("", menuFont, 64),
    missionDescription("", descriptionFont, 32),
    volumeLabel       ("", descriptionFont, 64)
{
    missionMenuOverlay.setPosition(510.f, 190.f);
    missionMenuOverlay.setFillColor(sf::Color::Black);
    missionTitle.setPosition(600.f, 580.f);
    missionTitle.setFillColor(sf::Color(0xCCCCCCFF));
    missionDescription.setPosition(600.f, 680.f);
    missionDescription.setFillColor(sf::Color(0xCCCCCCFF));

    missionThumbnail.setPosition(710.f, 280.f);

    menuFont.loadFromFile("assets/fonts/main_menu.ttf");
    descriptionFont.loadFromFile("assets/fonts/main_menu_description.ttf");

    logoTexture.loadFromFile("assets/textures/logo.png");
    logoSprite.setTexture(logoTexture, true);
    logoSprite.setPosition(50.f, 30.f);

    volumeLabel.setPosition(1410.f, 900.f);
}

const MainMenuEvent& MainMenu::getResponse() const {
    return response;
}

void MainMenu::onLoad(sf::RenderWindow& window) {
    window.setTitle("INJECTED! - MAIN MENU");
}

void MainMenu::updateMissionView(const int& index) {
    if (index >= missionPaths.size() || index < 0)
        return;
    
    const auto &path = missionPaths[index];

    std::string title;
    std::string description;

    std::getline(std::ifstream(path + "title.txt"), title);
    std::getline(std::ifstream(path + "description.txt"), description, '\0');

    missionTitle.setString(title);
    missionDescription.setString(description);

    missionThumbnailTexture.loadFromFile(path + "thumbnail.png");
    missionThumbnail.setTexture(missionThumbnailTexture, true);   
    const auto &size = missionThumbnailTexture.getSize();
    missionThumbnail.setScale(1.f / (size.x / 500.f), 1.f / (size.y / 250.f));

    currentMission = index;
}
