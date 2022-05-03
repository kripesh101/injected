#include <fstream>
#include "progression.hpp"
#include <filesystem.hpp>

// #include <iostream>
// using std::cout;

extern int volume;

bool Progression::loadCurrentStep(sf::RenderWindow& window) {
    const MissionStep& cStep = missionSteps[currentStep]; // Current Step
    const StepType& cType = cStep.type;
    const std::string cPath = missionPath + cStep.path; // Path specified in current step

    if (cType == StepType::LEVEL) {
        currentSim.reset(new Simulation(agent));
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

    else if (cType == StepType::MUSIC) {
        music.stop();
        music.openFromFile(cPath);
        music.setLoop(true);
        music.setVolume(30.f * volume / 100.f);
        music.play();
    }

    else if (cType == StepType::AUDIO) {
        audioClip.stop();
        audioClip.openFromFile(cPath);
        audioClip.setLoop(false);
        audioClip.setVolume(80.f * volume / 100.f);
        audioClip.play();
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

                window.setMouseCursorGrabbed(false);
            }
        }
    }
    // Screen fading
    else if (currentType == StepType::FADING) {
        if (fadeTime > 0.f) {
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
            const float fadeFactor = (1.f - (fadeTime / 2.f));
            fadeOverlay.setFillColor(sf::Color(0, 0, 0, 255 * fadeFactor));
            window.draw(fadeOverlay);
            
            // Fade out the audio clip
            audioClip.setVolume((1.f - fadeFactor) * 80.f * volume / 100.f);

            // If last step OR next step is music change, fade music
            if ((currentStep + 1) >= missionSteps.size() ||
                missionSteps[currentStep + 1].type == StepType::MUSIC
            ) {
                music.setVolume((1.f - fadeFactor) * 30.f * volume / 100.f);
            }

            if (window.hasFocus()) fadeTime -= deltaTime;

            if (fadeTime <= 0.f) {
                audioClip.stop();
            }
        
        } else {
            currentStep++;
            if (currentStep >= missionSteps.size()) {
                music.stop();
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
                music.stop();
                audioClip.stop();
                return true;
            } else if (response == RestartMenuEvent::RESTART) {
                window.setMouseCursorGrabbed(true);

                // Remove temporary LEVEL_FAIL
                missionSteps.erase(missionSteps.begin() + currentStep);
                // Reset step
                currentStep--;
                loadCurrentStep(window);
            }
        }
    }
    // Agent Selection
    else if (currentType == StepType::AGENT_SELECT) {
        if (characterSelect.update(window, mousePixelPos, deltaTime)) {
            agent = characterSelect.getSelectedAgent();

            // Skip immediately to next step
            currentType = StepType::FADING;
            fadeTime = 0.f;
        }
    }
    // Music Change
    else if (currentType == StepType::MUSIC || currentType == StepType::AUDIO) {
        // Skip immediately to next step, as music changing is handled separate of this update function
        currentType = StepType::FADING;
        fadeTime = 0.f;
    }

    return false;
}

bool Progression::onLoad(const std::string& missionFolderPath, sf::RenderWindow& window) {    
    missionPath = missionFolderPath;

    // if (ghc::filesystem::exists(missionPath + "music.txt")) {
    //     std::string musicFileName;
    //     std::getline(std::ifstream(missionPath + "music.txt"), musicFileName);
    //     if (!music.openFromFile(missionPath + musicFileName)) return false;
    //     music.setLoop(true);
    //     music.setVolume(30.f * volume / 100.f);
    //     music.play();
    // }
    
    std::ifstream file(missionFolderPath + "mission_details.txt");

    std::string type;
    std::string path;

    while (file >> type >> path) {
        MissionStep step;
        step.path = path;
        bool valid = true;

        if (type == "LEVEL")
            step.type = StepType::LEVEL;
        else if (type == "TRANSITION")
            step.type = StepType::TRANSITION;
        else if (type == "CREDITS")
            step.type = StepType::CREDITS;
        else if (type == "MUSIC")
            step.type = StepType::MUSIC;
        else if (type == "AUDIO")
            step.type = StepType::AUDIO;
        else
            valid = false;

        if (valid) missionSteps.push_back(step);
    }

    if (missionSteps.size() == 0) return false;

    // Character Select
    missionSteps.insert(missionSteps.begin(), {StepType::AGENT_SELECT});

    return loadCurrentStep(window);    
}

/*
sf::Font Progression::creditsFont = sf::Font();

void Progression::loadFont() {
    creditsFont.loadFromFile("assets/fonts/editor_text.ttf");
}
//*/

Progression::Progression() :
    view(sf::Vector2f(1920.f / 2, 1080.f / 2), sf::Vector2f(1920.f, 1080.f)),
    fadeOverlay(sf::Vector2f(1920.f, 1080.f)),
    currentStep(0),
    credits("", creditsFont, 40),
    currentSim(new Simulation(agent)),
    paused(false)
{
    creditsFont.loadFromFile("assets/fonts/credits.ttf");

    credits.setFillColor(sf::Color(0xEEEEEEFF));
    credits.setPosition(100.f, 100.f);

    fadeOverlay.setPosition(0.f, 0.f);
    transitionSprite.setPosition(0.f, 0.f);

    music.stop();
    audioClip.stop();
}

bool Progression::setPaused(const bool& pause) {
    switch (currentType) {
        case StepType::LEVEL_FAIL:
        case StepType::FADING:
            return false;
    }

    paused = pause;
    currentSim->setPaused(pause);

    if (pause) {
        music.pause();
        if (audioClip.getStatus() == sf::SoundSource::Status::Playing) audioClip.pause();
    } else {
        music.play();
        if (audioClip.getStatus() == sf::SoundSource::Status::Paused) audioClip.play();
    }
        

    return true;
}