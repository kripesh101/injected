#include <cmath>
#include <SFML/Graphics.hpp>
#include <filesystem.hpp>
#include "helper.hpp"

#include <common/level.hpp>
#include <common/utils.hpp>

namespace fs = ghc::filesystem;

int editorMain(const std::string& targetFolder) {
    // Create window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED! - MAP EDITOR", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    sf::View view(sf::Vector2f(140.f, 70.f), sf::Vector2f(300.f, 160.f));
    sf::Clock clock;
    EditorHelper helper;
    
    // Font for Editor Status Text
    sf::Font font;
    font.loadFromFile("assets/fonts/editor_text.ttf");
    const_cast<sf::Texture&>(font.getTexture(16)).setSmooth(false);
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(16);
    statusText.setFillColor(sf::Color::White);
    statusText.move(10.f, 10.f);

    // Level object
    Level lvl;

    // References to simplify code slightly
    TileMap &level = lvl.level;
    TileMap &walls = lvl.walls;

    // Preload decoration textures
    std::vector<FileTexture> &decorTextures = lvl.decorTextures;
    for (const auto & entry : fs::directory_iterator("assets/textures/decorations")) {
        FileTexture current;
        current.location = entry.path().string();
        current.loadFromFile(current.location);
        decorTextures.push_back(current);
    }

    // Load Level Data - Tiles and Decorations
    lvl.loadFromFolder(targetFolder, true);

    // TileMap outline
    const sf::Vector2u &size = level.getTileMapSize();
    sf::RectangleShape outline(sf::Vector2f(size.x * 16, size.y * 16));
    outline.setFillColor(sf::Color(0));
    outline.setOutlineThickness(5.f);
    outline.setOutlineColor(sf::Color(3, 252, 240));

    sf::Sprite decorPreview;
    decorPreview.setColor(sf::Color(255, 255, 255, 128));

    sf::Texture playerTex;
    playerTex.loadFromFile("assets/textures/editor/player_preview.png");
    sf::Sprite playerSpawn(playerTex);
    playerSpawn.setOrigin(8.f, 8.f);
    playerSpawn.setPosition(lvl.getPlayerSpawnPos());

    std::vector<Decoration>& decorations = lvl.decors;

    // Main Loop
    while (window.isOpen()) {
        float delta = clock.restart().asSeconds();

        const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        const sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    int tileId;

                    tileId = walls.getTile(mousePos.x, mousePos.y);
                    if (tileId) helper.setStatus(WALL_TILE, tileId);
                    else {
                        tileId = level.getTile(mousePos.x, mousePos.y);
                        if (tileId) helper.setStatus(LEVEL_TILE, tileId);
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (helper.getCurrentMode() == DECORATION) {
                        FileTexture& currentTex = decorTextures[helper.getCurrentTileNumber()];

                        Decoration decor;
                        decor.setTexture(currentTex, true);
                        decor.setPosition(decorPreview.getPosition());
                        decor.setRotation(decorPreview.getRotation());
                        decor.collidable = helper.isCollisionEnabled();
                        decor.texLocation = currentTex.location;
                        decorations.push_back(decor);
                    }
                    else if (helper.getCurrentMode() == PLAYER_SPAWN) {
                        auto viewPixelPos = sf::Vector2f(sf::Vector2i(mousePos));
                        lvl.playerSpawnPos = viewPixelPos;
                        playerSpawn.setPosition(viewPixelPos);
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (helper.getCurrentMode() == DECORATION) {
                        for (auto it = decorations.begin(); it != decorations.end(); ++it) {
                            if (it->getGlobalBounds().contains(mousePos)) {
                                decorations.erase(it);
                                break;
                            }
                        }
                    }
                }
            }

            // Decoration Preview Move
            if (event.type == sf::Event::MouseMoved) {
                if (helper.getCurrentMode() == DECORATION) {
                    auto viewPixelPos = sf::Vector2i(mousePos);
                    decorPreview.setPosition(viewPixelPos.x, viewPixelPos.y);
                }
            }

            // Zoom, Tile number, Rotation change
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    const float scrollDelta = event.mouseWheelScroll.delta;
                    
                    // LAlt -> Tile No. change
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
                        int currentTile = helper.getCurrentTileNumber();
                        
                        if (scrollDelta > 0) currentTile++;
                        else currentTile--;
                        
                        if (currentTile < 0) currentTile = 0;
                        
                        helper.setStatus(helper.getCurrentMode(), currentTile);
                    } 
                    // LCtrl -> Rotate Decoration
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                        float angle = 90.f;
                        // Finer rotation
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                            angle = std::abs(scrollDelta * 2.5f);
                        }

                        if (scrollDelta > 0) decorPreview.rotate(angle);
                        else decorPreview.rotate(-angle);
                    }
                    // Else Zoom
                    else {
                        const float zoomFactor = pow(2, -scrollDelta / 4);
                        view.zoom(zoomFactor);
                    }
                    
                }
            }

            // Non-Collidable Decorators are dimmed when pressing Q
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    for (auto& decor : decorations) {
                        if (!decor.collidable)
                            decor.setColor(sf::Color(255, 255, 255, 192));
                    }
                }

                // Press R to reset decoration rotation
                if (event.key.code == sf::Keyboard::R)
                    decorPreview.setRotation(0);
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Q) {
                    for (auto& decor : decorations) {
                        if (!decor.collidable)
                            decor.setColor(sf::Color::White);
                    }
                }
            }

            helper.processInputs(event);
        }

        if (window.hasFocus()) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (helper.getCurrentMode() == LEVEL_TILE)
                    level.changeTile(mousePos.x, mousePos.y, helper.getCurrentTileNumber());
                else if (helper.getCurrentMode() == WALL_TILE)
                    walls.changeTile(mousePos.x, mousePos.y, helper.getCurrentTileNumber());
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                if (helper.getCurrentMode() == LEVEL_TILE)
                    level.changeTile(mousePos.x, mousePos.y, 0);
                else if (helper.getCurrentMode() == WALL_TILE)
                    walls.changeTile(mousePos.x, mousePos.y, 0);
            }

            // Movement code
            float speed = 150.f * delta;
            
            if (!helper.isUserEntering()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    speed *= 5;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    view.move(speed, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    view.move(-speed, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    view.move(0.f, -speed);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    view.move(0.f, speed);
            }
        }

        // Changing background color
        window.clear(smoothColor(delta));

        // Draw TileMap and outline
        window.setView(view);
        window.draw(outline);
        
        window.draw(lvl);
        window.draw(playerSpawn);

        // Decoration Preview
        if (helper.getCurrentMode() == DECORATION) {          
            if (helper.getCurrentTileNumber() >= decorTextures.size())
                helper.setStatus(DECORATION, decorTextures.size() - 1);
            decorPreview.setTexture(decorTextures[helper.getCurrentTileNumber()], true);
            window.draw(decorPreview);
        }

        // Status Text
        window.setView(window.getDefaultView());
        helper.updateText(statusText);
        sf::FloatRect backgroundRect = statusText.getLocalBounds();
        sf::RectangleShape background(sf::Vector2f(backgroundRect.width, backgroundRect.height));
        background.setFillColor(sf::Color(0, 0, 0, 128));
        background.setOutlineColor(background.getFillColor());
        background.setOutlineThickness(5.f);
        window.draw(background, statusText.getTransform());
        window.draw(statusText);

        // Reset View
        window.setView(view);

        // Display
        window.display();
    }

    lvl.saveToFolder(targetFolder);

    return 0;
}
