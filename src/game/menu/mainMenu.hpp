#pragma once
#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "roundedRectangle.hpp"

struct MainMenuEvent {
    enum {
        QUIT,
        NEW_GAME
    } type;
    std::string missionPath;
};

class MainMenu {
private:
    sf::View view;

    sf::Font menuFont;
    Button newGame;
    Button quit;

    bool missionView = false;
    std::vector<std::string> missionPaths;
    int currentMission = 0;
    Button missionLeft;
    Button missionRight;
    Button missionPlay;
    Button missionBack;
    sf::RoundedRectangleShape missionMenuOverlay;
    sf::Texture missionThumbnailTexture;
    sf::Sprite missionThumbnail;
    sf::Text missionTitle;

    sf::Font descriptionFont;
    sf::Text missionDescription;

    sf::Texture logoTexture;
    sf::Sprite logoSprite;
    MainMenuEvent response;

    Button volumeUp;
    sf::Text volumeLabel;
    Button volumeDown;

    Button fullscreenToggle;

    void updateMissionView(const int& index);
public:
    MainMenu();

    const MainMenuEvent& getResponse() const;
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    void onLoad(sf::RenderWindow& window);
};