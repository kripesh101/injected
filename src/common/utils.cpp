#include "utils.hpp"
#include <cmath>
#include <cstdlib>

sf::Color smoothColor(const float& delta) {
    static float baseVal = 0.0f;
    baseVal += delta * 0.3f;
    const float r = std::abs(sin(baseVal)) * 96;
    const float g = std::abs(sin(baseVal + 2)) * 96;
    const float b = std::abs(sin(baseVal + 1)) * 96;

    return sf::Color(32 + r, g, 32 + b, 0);
}

int rand(int max, int min) {
    int rotation = max + 1 - min;
        while(rotation > max - min) 
            rotation = 1 + std::rand()/((RAND_MAX + 1u)/(max - min));

    return rotation + min;
}