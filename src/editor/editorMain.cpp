#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "helper.hpp"

using std::cout;

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
                constexpr float offset = 0.25f;

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
        constexpr float offset = 0.25f;

        quad[0].texCoords = sf::Vector2f(tu * tileSize.x + offset, tv * tileSize.y + offset);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, tv * tileSize.y + offset);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x - offset, (tv + 1) * tileSize.y - offset);
        quad[3].texCoords = sf::Vector2f(tu * tileSize.x + offset, (tv + 1) * tileSize.y - offset);
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

bool serialize(const std::string& file, int width, int height, int *data) {
    sf::Image img;
    img.create(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            img.setPixel(i, j, sf::Color(data[i + j * width], 0, 0));
        }
    }
    img.saveToFile(file);
}

bool deserialize(const std::string& file, int& width, int& height, const int& defaultSize, int*& data) {
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

int editorMain(const std::string& targetFolder) {
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED! - MAP EDITOR", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    int *levelArray;
    int *wallsArray;
    int width, height;

    deserialize(targetFolder + "wall.png", width, height, 300, wallsArray);
    deserialize(targetFolder + "level.png", width, height, 150, levelArray);

    TileMap level;
    TileMap walls;
    level.load("assets/textures/tileset.png", sf::Vector2u(16, 16), levelArray, width, height);
    walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), wallsArray, width * 2, height * 2);

    sf::View view(sf::Vector2f(140.f, 70.f), sf::Vector2f(300.f, 160.f));

    sf::Clock clock;

    EditorHelper helper;
    
    sf::Font font;
    font.loadFromFile("assets/fonts/editor_text.ttf");
    const_cast<sf::Texture&>(font.getTexture(16)).setSmooth(false);
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(16);
    statusText.setFillColor(sf::Color::White);
    statusText.move(10.f, 10.f);

    sf::RectangleShape outline(sf::Vector2f(width * 16, height * 16));
    outline.setFillColor(sf::Color(0));
    outline.setOutlineThickness(5.f);
    outline.setOutlineColor(sf::Color(3, 252, 240));

    while (window.isOpen()) {
        float delta = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f pos = window.mapPixelToCoords(pixelPos);

                    int tileId;

                    tileId = walls.getTile(pos.x, pos.y);
                    if (tileId) helper.setStatus(WALL_TILE, tileId);
                    else {
                        tileId = level.getTile(pos.x, pos.y);
                        if (tileId) helper.setStatus(LEVEL_TILE, tileId);
                    }
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    const float scrollDelta = event.mouseWheelScroll.delta;
                    
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
                        int currentTile = helper.getCurrentTileNumber();
                        
                        if (scrollDelta > 0) currentTile++;
                        else currentTile--;
                        
                        if (currentTile < 0) currentTile = 0;
                        helper.setStatus(helper.getCurrentMode(), currentTile);
                    } else {
                        const float zoomFactor = pow(2, -scrollDelta / 4);
                        view.zoom(zoomFactor);
                    }
                    
                }
            }

            helper.processInputs(event);
        }

        helper.updateText(statusText);

        if (window.hasFocus()) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                // get the current mouse position in the window
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

                // convert it to world coordinates
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                if (helper.getCurrentMode() == LEVEL_TILE)
                    level.changeTile(worldPos.x, worldPos.y, helper.getCurrentTileNumber());
                else if (helper.getCurrentMode() == WALL_TILE)
                    walls.changeTile(worldPos.x, worldPos.y, helper.getCurrentTileNumber());
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

        static float kekw = 0.0f;
        kekw += delta * 0.3f;
        const float r = std::abs(sin(kekw)) * 64;
        const float b = std::abs(cos(kekw + 1)) * 96;

        window.clear(sf::Color(32 + r, 0, 32 + b, 0));

        window.setView(view);
        window.draw(outline);
        window.draw(level);
        window.draw(walls);

        window.setView(window.getDefaultView());
        sf::FloatRect backgroundRect = statusText.getLocalBounds();
        sf::RectangleShape background(sf::Vector2f(backgroundRect.width, backgroundRect.height));
        background.setFillColor(sf::Color(0, 0, 0, 128));
        background.setOutlineColor(background.getFillColor());
        background.setOutlineThickness(5.f);

        window.draw(background, statusText.getTransform());
        window.draw(statusText);

        window.setView(view);

        window.display();
    }


    serialize(targetFolder + "level.png", width, height, levelArray);
    serialize(targetFolder + "wall.png", width * 2, height * 2, wallsArray);
    
    delete levelArray, wallsArray;

    return 0;
}
