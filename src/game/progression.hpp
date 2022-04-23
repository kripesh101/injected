#pragma once
#include "simulation.hpp"
#include "menu/restartLevelMenu.hpp"
#include "menu/characterSelect.hpp"
#include <memory>

enum class StepType {
    LEVEL,
    LEVEL_FAIL,
    TRANSITION,
    FADING,
    CREDITS,
    MUSIC,
    AGENT_SELECT
};

struct MissionStep {
    StepType type;
    std::string path;
};

class Progression {
public:
    Progression();
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

    std::unique_ptr<Simulation> currentSim;

    RestartMenu restartMenu;
    CharacterSelect characterSelect;

    Agent agent;

    bool paused;

    bool loadCurrentStep(sf::RenderWindow& window);
    std::vector<MissionStep> missionSteps;
    std::string missionPath;
    StepType currentType;
    int currentStep;
};