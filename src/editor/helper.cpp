#include "helper.hpp"

EditorMode EditorHelper::getCurrentMode() const {
    return currentMode;
}

int EditorHelper::getCurrentTileNumber() const {
    return currentTileNum;
}

int EditorHelper::isCollisionEnabled() const {
    return collisionEnabled;
}

bool EditorHelper::isUserEntering() const {
    return userEntering;
}

void EditorHelper::setStatus(EditorMode mode, int tileNum) {
    userEntering = false;
    currentTileNum = tileNum;
    currentMode = mode;
}

void EditorHelper::updateText(sf::Text& text) const {
    sf::String str = "Current: ";

    textHelper(str, currentMode, currentTileNum);

    if (userEntering) {
        str += "\nNext: ";
        textHelper(str, nextMode, nextTileNum);
        if (nextMode == NONE) str += " [W - Wall, T - Level, D - Decoration, P - Player Spawn, E - Enemy, H - Heart]";
    }

    text.setString(str);
}

void EditorHelper::textHelper(sf::String& str, EditorMode mode, int tileNum) const {
    const std::string tile = std::to_string(tileNum);
    switch (mode)
    {
        case NONE:
            str += "None";
            break;
        case WALL_TILE:
            str += "Wall - " + tile;
            break;
        case LEVEL_TILE:
            str += "Level - " + tile;
            break;
        case DECORATION:
            str += "Decor - " + tile + " | " + (collisionEnabled ? "Collidable" : "Not Collidable");
            break;
        case PLAYER_SPAWN:
            str += "Player Spawn";
            break;
        case ENEMY:
            str += "Enemy";
            break;
        case HEART:
            str += "Heart";
    }
}

void EditorHelper::processInputs(const sf::Event& event) {
    if (nextMode != NONE && userEntering) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= 48 && event.text.unicode <= 57) {
                nextTileNum = nextTileNum * 10 + (event.text.unicode - 48);
            }
        }    
    }


    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::C) {
            collisionEnabled = !collisionEnabled;
        }

        if (userEntering) {
            // Cancel input
            if (event.key.code == sf::Keyboard::Escape) {
                userEntering = false;
            }

            // Apply input
            if (event.key.code == sf::Keyboard::Enter) {
                currentMode = nextMode;
                currentTileNum = nextTileNum;
                userEntering = false;
            }

            if (event.key.code == sf::Keyboard::W) {
                nextMode = WALL_TILE;
            }

            if (event.key.code == sf::Keyboard::T) {
                nextMode = LEVEL_TILE;
            }
            if (event.key.code == sf::Keyboard::L) {
                nextMode = LEVEL_TILE;
            }

            if (event.key.code == sf::Keyboard::D) {
                nextMode = DECORATION;
            }

            if (event.key.code == sf::Keyboard::P) {
                nextMode = PLAYER_SPAWN;
            }

            if (event.key.code == sf::Keyboard::E) {
                nextMode = ENEMY;
            }

            if (event.key.code == sf::Keyboard::H) {
                nextMode = HEART;
            }

            if (event.key.code == sf::Keyboard::Backspace) {
                nextTileNum = nextTileNum / 10;
            }
        } else {
            if (event.key.code == sf::Keyboard::Tilde) {
                nextMode = NONE;
                nextTileNum = 0;
                userEntering = true;
            }
        }
    }
}
