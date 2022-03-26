#include "tilemap.hpp"
#include <cmath>

bool TileMap::load(const std::string& tileset, sf::Vector2u tilesize2, int* tilesData, unsigned int width, unsigned int height) {
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

bool TileMap::collides(const sf::FloatRect& bb) const {
    // cout << "\n";
    return pointCollides(bb.left, bb.top) || pointCollides(bb.left + bb.width / 2, bb.top) 
        || pointCollides(bb.left + bb.width, bb.top) || pointCollides(bb.left + bb.width, bb.top + bb.height / 2) 
        || pointCollides(bb.left + bb.width, bb.top + bb.height) || pointCollides(bb.left + bb.width / 2, bb.top + bb.height)
        || pointCollides(bb.left, bb.top + bb.height) || pointCollides(bb.left, bb.top + bb.height / 2);
}

bool TileMap::collides(const sf::Vector2f& point) const {
    return pointCollides(point.x, point.y);
}

int TileMap::getTile(const float& x, const float& y) {
        const sf::Vector2i tile = getTileCoordinates(x, y);
        
        // Tile coordinates outside of tilemap
        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return 0;

        const int currentTile = tile.x + tile.y * tilemapSize.x;
        return tiles[currentTile];
    }

void TileMap::changeTile(const float& x, const float& y, const int& tileNumber) {
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

bool TileMap::pointLiesWithin(sf::Vector2f point) {
    const sf::Vector2i tile = getTileCoordinates(point.x, point.y);

    if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
        return false;
    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

bool TileMap::pointCollides(const float& x, const float& y) const {
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

sf::Vector2i TileMap::getTileCoordinates(float x, float y) const {
    // const sf::Vector2f& tilemapPos = getPosition();
    // x -= tilemapPos.x;
    // y -= tilemapPos.y;

    return sf::Vector2i(floor(x / tileSize.x), floor(y / tileSize.y));
}

const sf::Vector2u& TileMap::getTileMapSize() const {
    return tilemapSize;
}

int* TileMap::getTilesData() const {
    return tiles;
}

bool deserializeTileData(const std::string& file, int& width, int& height, int*& data) {
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
    else
        return false;
}

void serializeTileData(const std::string& file, const int& width, const int& height, const int* data) {
    sf::Image img;
    img.create(width, height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            img.setPixel(i, j, sf::Color(data[i + j * width], 0, 0));
        }
    }
    img.saveToFile(file);
}
