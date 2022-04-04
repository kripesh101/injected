#include <fstream>
#include "progression.hpp"

// #include <iostream>
// using std::cout;

bool Progression::loadCurrentStep(sf::RenderWindow& window) {
    const MissionStep& cStep = missionSteps[currentStep]; // Current Step
    const StepType& cType = cStep.type;
    const std::string cPath = missionPath + cStep.path; // Path specified in current step

    if (cType == StepType::LEVEL) {
        delete currentSim;
        currentSim = new Simulation();
        if (!currentSim->onLoad(cPath, window)) return false;
    }
    
    else if (cType == StepType::TRANSITION) {
        if (!transitionTex.loadFromFile(cPath)) return false;
        transitionSprite.setTexture(transitionTex, true);

        const auto &size = transitionTex.getSize();
        transitionSprite.setScale(1.f / (size.x / 1920.f), 1.f / (size.y / 1080.f));
    }

    else if (cType == StepType::CREDITS) {
        std::string text;
        std::getline(std::ifstream(cPath), text, '\0');
        credits.setString(text);
    }

    currentType = cType;

    return true;
}

bool Progression::update(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const float& deltaTime) {
    // Playing Level
    if (currentType == StepType::LEVEL) {
        if (currentSim->update(window, mousePixelPos, deltaTime)) {
            const auto &simEvent = currentSim->getEvent();
            fadeTime = 2.f;
            currentType = StepType::FADING;

            if (simEvent == SimulationEvent::PLAYER_DEAD) {
                auto newPos = missionSteps.begin() + (currentStep + 1);
                MissionStep newStep;
                newStep.type = StepType::LEVEL_FAIL;
                missionSteps.insert(newPos, newStep);
            }
        }
    }
    // Screen fading
    else if (currentType == StepType::FADING) {
        if (fadeTime > 0) {
            if (missionSteps[currentStep].type == StepType::LEVEL)  {
                currentSim->update(window, mousePixelPos, deltaTime);
            }
            else if (missionSteps[currentStep].type == StepType::TRANSITION) {
                window.setView(view);
                window.draw(transitionSprite);
            } else if (missionSteps[currentStep].type == StepType::CREDITS) {
                window.setView(view);
                window.draw(credits);
            }

            window.setView(view);
            fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255 * (1.f - (fadeTime / 2.f))));
            window.draw(fadeOverlay);
            
            if (!paused) fadeTime -= deltaTime;
        
        } else {
            currentStep++;
            if (currentStep >= missionSteps.size()) {
                // Game Completely Finished
                return true;
            } else {
                if (!loadCurrentStep(window)) { 
                    return true;
                }
            }
        }
    }
    // Transition Screen
    else if (currentType == StepType::TRANSITION) {
        if (!paused && window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentType = StepType::FADING;
            fadeTime = 2.f;
        }
        
        window.setView(view);
        window.draw(transitionSprite);
    }
    // Credits screen
    else if (currentType == StepType::CREDITS) {
        if (!paused && window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentType = StepType::FADING;
            fadeTime = 2.f;
        }

        window.setView(view);
        window.draw(credits);
    }
    // Restart Level screen - on level fail
    else if (currentType == StepType::LEVEL_FAIL) {
        if (restartMenu.update(window, mousePixelPos, deltaTime)) {
            const auto& response = restartMenu.getResponse();
            if (response == RestartMenuEvent::QUIT) {
                window.close();
                return true;
            } else if (response == RestartMenuEvent::RETURN_MAIN_MENU) {
                return true;
            } else if (response == RestartMenuEvent::RESTART) {
                // Remove temporary LEVEL_FAIL
                missionSteps.erase(missionSteps.begin() + currentStep);
                // Reset step
                currentStep--;
                loadCurrentStep(window);
            }
        }

    }

    return false;
}

bool Progression::onLoad(const std::string& missionFolderPath, sf::RenderWindow& window) {    
    missionPath = missionFolderPath;
    std::ifstream file(missionPath + "mission_details.txt");

    std::string type;
    std::string path;

    while (file >> type >> path) {
        MissionStep step;
        step.path = path;

        if (type == "LEVEL")
            step.type = StepType::LEVEL;
        else if (type == "TRANSITION")
            step.type = StepType::TRANSITION;
        else if (type == "CREDITS")
            step.type = StepType::CREDITS;

        missionSteps.push_back(step);
    }

    if (missionSteps.size() == 0) return false;
    return loadCurrentStep(window);    
}

/*
sf::Font Progression::creditsFont = sf::Font();

void Progression::loadFont() {
    creditsFont.loadFromFile("assets/fonts/editor_text.ttf");
}
//*/

Progression::~Progression() {
    delete currentSim;
}

Progression::Progression() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f, 1080.f)),
    fadeOverlay(sf::Vector2f(1920.f, 1080.f)),
    currentStep(0),
    credits("", creditsFont, 40),
    currentSim(new Simulation()),
    paused(false)
{
    creditsFont.loadFromFile("assets/fonts/credits.ttf");

    credits.setFillColor(sf::Color(0xEEEEEEFF));
    credits.setPosition(100.f, 100.f);

    fadeOverlay.setPosition(0.f, 0.f);
    transitionSprite.setPosition(0.f, 0.f);
}

bool Progression::setPaused(const bool& pause) {
    switch (currentType) {
        case StepType::LEVEL_FAIL:
        case StepType::FADING:
            return false;
    }

    paused = pause;
    currentSim->setPaused(pause);
    return true;
}