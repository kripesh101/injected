#pragma once

#include "decoration.hpp"
#include "tilemap.hpp"
#include <vector>

struct EnemyData {
    sf::Vector2f position;
    float rotation;
};

class Level : public sf::Drawable {
public:
    /**
     * @param fallback when true, placeholder arrays are created for the tilemap data if it doesn't exist.
     * @returns true if loading was successful, false if loading failed.
    */
    bool loadFromFolder(const std::string& folderPath, const bool fallback = false);

    void saveToFolder(const std::string& folderPath) const;
    
    // handles decors and walls collision

    bool collides(const sf::Vector2f& point, const bool& ignoreDecors = false) const;
    bool collides(const sf::FloatRect& boundingBox, const bool& ignoreDecors = false) const;

    bool lineCollides(const sf::Vector2f& pointA, const sf::Vector2f& pointB, const bool& ignoreDecors = false) const;

    // other static data related to level, used in simulation

    const sf::Vector2f& getPlayerSpawnPos() const;
    const std::vector<EnemyData>& getEnemyData() const;


    friend int editorMain(const std::string& targetFolder);
    
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Decoration> decors;
    std::vector<FileTexture> decorTextures;
    TileMap level;
    TileMap walls;
    
    sf::FloatRect levelBounds;

    sf::Vector2f playerSpawnPos = sf::Vector2f(-32.0f, -32.0f);
    std::vector<EnemyData> enemies;
};