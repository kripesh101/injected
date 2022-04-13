#include "level.hpp"

#include <filesystem.hpp>
#include <cmath>
namespace fs = ghc::filesystem;

void deserializeEnemyData(const std::string& filePath, std::vector<EnemyData>& enemies) {
    if (!fs::exists(filePath)) return;

    std::ifstream file(filePath);
    float posX, posY, rot;

    while (file >> posX >> posY >> rot) {
        EnemyData enemy;
        enemy.position = sf::Vector2f(posX, posY);
        enemy.rotation = rot;
        enemies.push_back(enemy);
    }
}

void serializeEnemyData(const std::string& filePath, const std::vector<EnemyData>& enemies) {
    std::ofstream file(filePath);

    for (const auto& enemy : enemies) {
        file << enemy.position.x << " "
        << enemy.position.y << " "
        << enemy.rotation << "\n";
    }

    file.close();
}

bool Level::loadFromFolder(const std::string& folderPath, const bool fallback) {
    if (fallback) fs::create_directories(folderPath);
    
    int *levelData;

    // Level TileMap
    int tilesWidth = 150, tilesHeight = 150;
    if (!deserializeTileData(folderPath + "level.png", tilesWidth, tilesHeight, levelData)) {        
        if (fallback) levelData = new int[tilesWidth * tilesHeight]();
        else return false;
    }
    // Load level-specific tileset texture if it exists
    if (fs::exists(folderPath + "tileset.png"))
        level.load(folderPath + "tileset.png", sf::Vector2u(16, 16), levelData, tilesWidth, tilesHeight);
    else
        level.load("assets/textures/tileset.png", sf::Vector2u(16, 16), levelData, tilesWidth, tilesHeight);

    // Walls TileMap
    tilesWidth = 300, tilesHeight = 300;
    if (!deserializeTileData(folderPath + "wall.png", tilesWidth, tilesHeight, levelData)) {
        if (fallback) levelData = new int[tilesWidth * tilesHeight]();
        else return false;
    }
    // Load level-specific wall tileset texture if it exists
    if (fs::exists(folderPath + "wall_tileset.png"))
        walls.load(folderPath + "wall_tileset.png", sf::Vector2u(8, 8), levelData, tilesWidth, tilesHeight);
    else
        walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), levelData, tilesWidth, tilesHeight);

    // Decorations
    if (!deserializeDecorations(folderPath + "decor.txt", decors, decorTextures)) {
        if (!fallback) return false;
    }

    levelBounds = sf::FloatRect(-64, -64, tilesWidth * 8 + 128, tilesHeight * 8 + 128);

    if (fs::exists(folderPath + "player_spawn.txt")) {
        std::ifstream playerSpawn(folderPath + "player_spawn.txt");
        playerSpawn >> playerSpawnPos.x >> playerSpawnPos.y;
        playerSpawn.close();
    }

    deserializeEnemyData(folderPath + "enemy_spawns.txt", enemies);

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

    serializeEnemyData(folderPath + "enemy_spawns.txt", enemies);
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

const std::vector<EnemyData>& Level::getEnemyData() const {
    return enemies;
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

bool Level::lineCollides(const sf::Vector2f& pointA, const sf::Vector2f& pointB, const bool& ignoreDecors) const {
    const sf::Vector2f AtoB = pointB - pointA;
    float distance = std::sqrt(AtoB.x * AtoB.x + AtoB.y * AtoB.y);
    const sf::Vector2f AtoBUnit = AtoB / distance;

    sf::Vector2f currentPoint = pointA;

    if (collides(currentPoint, ignoreDecors)) return true;

    while (distance > 0.f) {
        if (distance >= 8.f) {
            currentPoint += AtoBUnit * 8.f;
            distance -= 8.f;
        } else {
            currentPoint += AtoBUnit * distance;
            distance = 0.f;
        }

        if (collides(currentPoint, ignoreDecors)) return true;
    }

    return false;
}