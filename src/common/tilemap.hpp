#pragma once
#include <SFML/Graphics.hpp>

constexpr int defaultTileMapSize = 150;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tilesize2, int* tilesData, unsigned int width, unsigned int height);
    bool collides(const sf::FloatRect& bb);
    void changeTile(const float& x, const float& y, const int& tileNumber);
    int getTile(const float& x, const float& y);
    bool pointLiesWithin(sf::Vector2f point);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool pointCollides(const float& x, const float& y) const;
    sf::Vector2i getTileCoordinates(float x, float y) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u tileSize;
    sf::Vector2u tilemapSize;

    int* tiles;
};

bool deserializeTileData(const std::string& file, int& width, int& height, int*& data);
bool serializeTileData(const std::string& file, const int& width, const int& height, const int* data);