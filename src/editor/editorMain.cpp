#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <filesystem.hpp>
#include "helper.hpp"

using std::cout;
namespace fs = ghc::filesystem;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tilesize2, int* tilesData, unsigned int width, unsigned int height)
    {
        tileSize = tilesize2;
        tilemapSize = sf::Vector2u(width, height);
        tiles = tilesData;

        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;
        m_tileset.setSmooth(false);

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                constexpr float offset = 0.1f;

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x + offset, tv * tileSize.y + offset);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, tv * tileSize.y + offset);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, (tv + 1) * tileSize.y - offset);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x + offset, (tv + 1) * tileSize.y - offset);
            }

        return true;
    }

    int getTile(const float& x, const float& y) {
        const sf::Vector2i tile = getTileCoordinates(x, y);
        
        // Tile coordinates outside of tilemap
        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return 0;

        const int currentTile = tile.x + tile.y * tilemapSize.x;
        return tiles[currentTile];
    }

    void changeTile(const float& x, const float& y, const int& tileNumber) {
        const sf::Vector2i tile = getTileCoordinates(x, y);
        
        // Tile coordinates outside of tilemap
        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return;

        const int currentTile = tile.x + tile.y * tilemapSize.x;
        tiles[currentTile] = tileNumber;

        int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
        int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

        sf::Vertex* quad = &m_vertices[currentTile * 4];

        // update its 4 texture coordinates
        constexpr float offset = 0.1f;

        quad[0].texCoords = sf::Vector2f(tu * tileSize.x + offset, tv * tileSize.y + offset);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, tv * tileSize.y + offset);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, (tv + 1) * tileSize.y - offset);
        quad[3].texCoords = sf::Vector2f(tu * tileSize.x + offset, (tv + 1) * tileSize.y - offset);
    }

    bool pointLiesWithin(sf::Vector2f point) {
        const sf::Vector2i tile = getTileCoordinates(point.x, point.y);

        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return false;
        return true;
    }
private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    bool pointCollides(const float& x, const float& y) const {
        const sf::Vector2i tile = getTileCoordinates(x, y);
        
        // cout << tile.x << " " << tile.y << " | ";

        // Tile coordinates outside of tilemap
        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return false;

        // If tile data is 0, no collision.
        // Else collide.
        if (tiles[tile.x + tile.y * tilemapSize.x] == 0) return false;
        else return true;
    }

    sf::Vector2i getTileCoordinates(float x, float y) const {
        // const sf::Vector2f& tilemapPos = getPosition();
        // x -= tilemapPos.x;
        // y -= tilemapPos.y;

        return sf::Vector2i(floor(x / tileSize.x), floor(y / tileSize.y));
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u tileSize;
    sf::Vector2u tilemapSize;

    int* tiles;
};

static bool serialize(const std::string& file, int width, int height, int* data) {
    sf::Image img;
    img.create(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            img.setPixel(i, j, sf::Color(data[i + j * width], 0, 0));
        }
    }
    img.saveToFile(file);
}

static bool deserialize(const std::string& file, int& width, int& height, const int& defaultSize, int*& data) {
    sf::Image img;
    if (img.loadFromFile(file)) {
        auto size = img.getSize();
        width = size.x;
        height = size.y;
        data = new int[width * height]();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                data[i + j * width] = img.getPixel(i, j).r;
            }
        }
        return true;
    }
    else {
        width = defaultSize;
        height = defaultSize;
        data = new int[width * height]();
        return false;
    } 
}

struct Texture : public sf::Texture {
    std::string location;
};

struct Decoration : public sf::Sprite {
    bool collidable = false;
    std::string texLocation;
};

static void serializeDecorations(const std::string& filePath, std::vector<Decoration>& array) {
    std::ofstream file(filePath);

    for (const auto& decor : array) {
        file << decor.texLocation << " "
        << decor.getPosition().x << " "
        << decor.getPosition().y << " "
        << decor.getRotation() << " "
        << decor.collidable << "\n";
    }

    file.close();
}

static void deserializeDecorations(const std::string& filePath, std::vector<Decoration>& array, const std::vector<Texture>& textures) {
    if (!fs::exists(filePath)) return;

    std::ifstream file(filePath);

    std::string texLoc;
    float posX, posY, rot;
    bool collidable;

    while (file >> texLoc >> posX >> posY >> rot >> collidable) {
        Decoration current;
        current.texLocation = texLoc;
        current.collidable = collidable;

        current.setPosition(posX, posY);
        current.setRotation(rot);

        bool found = false;
        for (const auto& tex : textures) {
            if (tex.location == current.texLocation) {
                current.setTexture(tex, true);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Invalid texture referenced by decoration. Texture file path: " << current.texLocation << "\n";
        } else {
            array.push_back(current);
        }
    }
}

int editorMain(const std::string& targetFolder) {
    // Create window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED! - MAP EDITOR", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // TileMap Data allocations
    int *levelArray;
    int *wallsArray;
    int width, height;

    // Load TileMap data
    fs::create_directories(targetFolder);
    deserialize(targetFolder + "wall.png", width, height, 300, wallsArray);
    deserialize(targetFolder + "level.png", width, height, 150, levelArray);

    // TileMap objects
    TileMap level;
    TileMap walls;
    level.load("assets/textures/tileset.png", sf::Vector2u(16, 16), levelArray, width, height);
    walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), wallsArray, width * 2, height * 2);

    sf::View view(sf::Vector2f(140.f, 70.f), sf::Vector2f(300.f, 160.f));
    sf::Clock clock;
    EditorHelper helper;
    
    // Font
    sf::Font font;
    font.loadFromFile("assets/fonts/editor_text.ttf");
    const_cast<sf::Texture&>(font.getTexture(16)).setSmooth(false);
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(16);
    statusText.setFillColor(sf::Color::White);
    statusText.move(10.f, 10.f);

    // TileMap outline
    sf::RectangleShape outline(sf::Vector2f(width * 16, height * 16));
    outline.setFillColor(sf::Color(0));
    outline.setOutlineThickness(5.f);
    outline.setOutlineColor(sf::Color(3, 252, 240));

    // Decorations
    std::vector<Texture> decorTextures;
    for (const auto & entry : fs::directory_iterator("assets/textures/decorations")) {
        Texture current;
        current.location = entry.path().string();
        current.loadFromFile(current.location);
        decorTextures.push_back(current);
    }

    sf::Sprite decorPreview;
    decorPreview.setColor(sf::Color(255, 255, 255, 128));

    std::vector<Decoration> decorations;
    deserializeDecorations(targetFolder + "decor.txt", decorations, decorTextures);

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
                        Texture& currentTex = decorTextures[helper.getCurrentTileNumber()];

                        Decoration decor;
                        decor.setTexture(currentTex, true);
                        decor.setPosition(decorPreview.getPosition());
                        decor.setRotation(decorPreview.getRotation());
                        decor.collidable = helper.isCollisionEnabled();
                        decor.texLocation = currentTex.location;
                        decorations.push_back(decor);
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (helper.getCurrentMode() == DECORATION) {
                        for (auto it = decorations.begin(); it != decorations.end(); ++it) {
                            if (it->getGlobalBounds().contains(mousePos)) {
                                decorations.erase(it);
                                it--;
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
        static float kekw = 0.0f;
        kekw += delta * 0.3f;
        const float r = std::abs(sin(kekw)) * 64;
        const float b = std::abs(cos(kekw + 1)) * 96;
        window.clear(sf::Color(32 + r, 0, 32 + b, 0));

        // Draw TileMap and outline
        window.setView(view);
        window.draw(outline);
        window.draw(level);
        window.draw(walls);

        // Decorations
        for (const auto& decor : decorations)
            window.draw(decor);

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


    serialize(targetFolder + "level.png", width, height, levelArray);
    serialize(targetFolder + "wall.png", width * 2, height * 2, wallsArray);

    serializeDecorations(targetFolder + "decor.txt", decorations);
    
    delete levelArray, wallsArray;

    return 0;
}
