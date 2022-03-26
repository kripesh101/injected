#include "utils.hpp"
#include <cmath>

sf::Color smoothColor(const float& delta) {
    static float baseVal = 0.0f;
    baseVal += delta * 0.3f;
    const float r = std::abs(sin(baseVal)) * 64;
    const float b = std::abs(cos(baseVal + 1)) * 96;

    return sf::Color(32 + r, 0, 32 + b, 0);
}
