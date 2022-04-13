#pragma once
#include "simulation.hpp"
#include "menu/restartLevelMenu.hpp"

enum class StepType {
    LEVEL,
    LEVEL_FAIL,
    TRANSITION,
    FADING,
    CREDITS
};

struct MissionStep {
    StepType type;
    std::string path;
};

class Progression {
public:
    Progression();
    ~Progression();
    bool update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime);
    bool onLoad(const std::string& missionFolderPath, sf::RenderWindow& window);
    bool setPaused(const bool& pause);
private:
    sf::View view;

    float fadeTime;
    sf::RectangleShape fadeOverlay;

    sf::Texture transitionTex;
    sf::Sprite transitionSprite;

    sf::Text credits;
    sf::Font creditsFont;

    sf::Music music;

    Simulation *currentSim;

    RestartMenu restartMenu;

    bool paused;

    bool loadCurrentStep(sf::RenderWindow& window);
    std::vector<MissionStep> missionSteps;
    std::string missionPath;
    StepType currentType;
    int currentStep;
};