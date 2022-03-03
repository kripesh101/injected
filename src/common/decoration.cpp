#include "decoration.hpp"

#include <fstream>
#include <iostream>
#include <filesystem.hpp>
namespace fs = ghc::filesystem;

bool Decoration::collidesWith(sf::FloatRect bb) const {
    return collidable && getGlobalBounds().intersects(bb);
}

bool deserializeDecorations(const std::string& filePath, std::vector<Decoration>& decorations, std::vector<FileTexture>& textures) {
    if (!fs::exists(filePath)) return false;
    
    std::ifstream file(filePath);

    std::string texLoc;
    float posX, posY, rot;
    bool collidable;

    while (file >> texLoc >> posX >> posY >> rot >> collidable) {
        Decoration current;
        current.collidable = collidable;
        current.texLocation = texLoc;
        current.setPosition(posX, posY);
        current.setRotation(rot);

        bool found = false;
        for (const auto& tex : textures) {
            if (tex.location == texLoc) {
                found = true;
                break;
            }
        }

        if (!found) {
            FileTexture tex;
            if (!tex.loadFromFile(texLoc)) {
                std::cout << 
                    "Invalid or non-existing texture referenced by decoration. Texture file path: " <<
                    current.texLocation << "\n";
                return false;
            }
            tex.location = texLoc;
            textures.push_back(tex);
        }
        decorations.push_back(current);
    }

    for (auto& decor : decorations) {
        for (const auto& tex : textures) {
            if (tex.location == decor.texLocation) {
                decor.setTexture(tex, true);
                break;
            }
        }
    }

    return true;
}

void serializeDecorations(const std::string& filePath, std::vector<Decoration>& decorations) {
    std::ofstream file(filePath);

    for (const auto& decor : decorations) {
        file << decor.texLocation << " "
        << decor.getPosition().x << " "
        << decor.getPosition().y << " "
        << decor.getRotation() << " "
        << decor.collidable << "\n";
    }

    file.close();
}

bool decorCollides(const sf::FloatRect& bb, const std::vector<Decoration>& array) {
    for (const auto& decor: array) {
        if (decor.collidesWith(bb)) return true;
    }
    return false;
}
