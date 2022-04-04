#include "level.hpp"

#include <filesystem.hpp>

bool Level::loadFromFolder(const std::string& folderPath, const bool fallback) {
    if (fallback) ghc::filesystem::create_directories(folderPath);
    
    int *levelData;

    // Level TileMap
    int tilesWidth = 150, tilesHeight = 150;
    if (!deserializeTileData(folderPath + "level.png", tilesWidth, tilesHeight, levelData)) {        
        if (fallback) levelData = new int[tilesWidth * tilesHeight]();
        else return false;
    }
    level.load("assets/textures/tileset.png", sf::Vector2u(16, 16), levelData, tilesWidth, tilesHeight);

    // Walls TileMap
    tilesWidth = 300, tilesHeight = 300;
    if (!deserializeTileData(folderPath + "wall.png", tilesWidth, tilesHeight, levelData)) {
        if (fallback) levelData = new int[tilesWidth * tilesHeight]();
        else return false;
    }
    walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), levelData, tilesWidth, tilesHeight);

    // Decorations
    if (!deserializeDecorations(folderPath + "decor.txt", decors, decorTextures)) {
        if (!fallback) return false;
    }

    levelBounds = sf::FloatRect(-64, -64, tilesWidth * 8 + 128, tilesHeight * 8 + 128);

    if (ghc::filesystem::exists(folderPath + "player_spawn.txt")) {
        std::ifstream playerSpawn(folderPath + "player_spawn.txt");
        playerSpawn >> playerSpawnPos.x >> playerSpawnPos.y;
        playerSpawn.close();
    }

    return true;
}

void Level::saveToFolder(const std::string& folderPath) const {
    auto& sizeLvl = level.getTileMapSize();
    serializeTileData(folderPath + "level.png", sizeLvl.x, sizeLvl.y, level.getTilesData());

    auto& sizeWalls = walls.getTileMapSize();
    serializeTileData(folderPath + "wall.png", sizeWalls.x, sizeWalls.y, walls.getTilesData());

    serializeDecorations(folderPath + "decor.txt", decors);

    std::ofstream spawnFile(folderPath + "player_spawn.txt");
    spawnFile << playerSpawnPos.x << " " << playerSpawnPos.y;
    spawnFile.close();
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(level, states);
    target.draw(walls, states);

    for (const auto& decor : decors)
        target.draw(decor, states);
}

const sf::Vector2f& Level::getPlayerSpawnPos() const {
    return playerSpawnPos;
}


bool Level::collides(const sf::Vector2f& point, const bool& ignoreDecors) const {
    if (ignoreDecors || levelBounds.contains(point))
        return walls.collides(point) || ((ignoreDecors) ? false : decorCollides(point, decors));
    else return true;
}

bool Level::collides(const sf::FloatRect& boundingBox, const bool& ignoreDecors) const {
    if (ignoreDecors || levelBounds.intersects(boundingBox))
        return walls.collides(boundingBox) || ((ignoreDecors) ? false : decorCollides(boundingBox, decors));
    else return true;
}
