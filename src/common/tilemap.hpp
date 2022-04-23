#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

constexpr int defaultTileMapSize = 150;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tilesize2, std::shared_ptr<int []>& tilesData, unsigned int width, unsigned int height);
    
    bool collides(const sf::FloatRect& bb) const;
    bool collides(const sf::Vector2f& point) const;
    
    void changeTile(const float& x, const float& y, const int& tileNumber);
    int getTile(const float& x, const float& y);
    bool pointLiesWithin(sf::Vector2f point);

    const sf::Vector2u& getTileMapSize() const;
    std::shared_ptr<int []> getTilesData() const;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool pointCollides(const float& x, const float& y) const;
    sf::Vector2i getTileCoordinates(float x, float y) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u tileSize;
    sf::Vector2u tilemapSize;

    std::shared_ptr<int []> tiles;
};

bool deserializeTileData(const std::string& file, int& width, int& height, std::shared_ptr<int []>& data);
void serializeTileData(const std::string& file, const int& width, const int& height, const std::shared_ptr<int []>& data);
