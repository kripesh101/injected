#include "player.hpp"
#include <cmath>

// #include <iostream>
// using std::cout;

Player::Player() : 
    boundingBox(sf::Vector2f(16.f, 16.f)),
    view(sf::Vector2f(0.f, 0.f), sf::Vector2f(450.f, 250.f)),
    legs(sf::seconds(0.06f)),
    speed(120.f)
{
    setOrigin(24.f, 24.f);
    setPosition(sf::Vector2f(0.f, 0.f));

    for (int i = 0; i < 9; i++) {
        legAnimation.addFrame(sf::IntRect(i * 32, 0, 32, 16));
    }
    legs.setPosition(0.f, 0.f);
    legs.setOrigin(16.f, 8.f);
}

void Player::setTextures(const sf::Texture& player, const sf::Texture& legTex) {
    sf::Sprite::setTexture(player, true);
    legAnimation.setSpriteSheet(legTex);
}

void Player::setPosition(const sf::Vector2f& pos) {
    sf::Sprite::setPosition(pos);
    legs.setPosition(pos);
    view.setCenter(pos);
}

void Player::processInput(sf::RenderWindow& window, const sf::Vector2i& mousePixelPos, const Level& level, const float& delta) {
    // Movement
    float moved = speed * delta;

    const bool MOVE_UP    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    const bool MOVE_DOWN  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    const bool MOVE_LEFT  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    const bool MOVE_RIGHT = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    const bool playerMoved = (MOVE_LEFT != MOVE_RIGHT) || (MOVE_UP != MOVE_DOWN);
    const bool diagonal = (MOVE_LEFT || MOVE_RIGHT) && (MOVE_UP || MOVE_DOWN);

    // Leg rotations
    float legRotation = 0.f;
    if (MOVE_UP != MOVE_DOWN) {
        legRotation = 90.f;
        if (MOVE_LEFT != MOVE_RIGHT) {
            if (MOVE_LEFT && MOVE_UP || MOVE_RIGHT && MOVE_DOWN)
                legRotation -= 45.f;
            else
                legRotation += 45.f;
        }
    }
    
    
    // If moving diagonally, nerf speed slightly
    if (diagonal) moved *= 0.75f;

    sf::Vector2f oldPos = getPosition();

    if (MOVE_RIGHT) {
        move(moved, 0.f);
    }
    if (MOVE_LEFT) {
        move(-moved, 0.f);
    }

    if (level.collides(getGlobalBounds())) {
        // Get new (collided) position
        const sf::Vector2f newPos = getPosition(); 
        
        // Restore to non-collided position
        setPosition(oldPos);

        if (!diagonal) {
            for (int i = 1; i <= 12; i++) {
                auto const offset = sf::Vector2f(0.f, i);
                auto tempBounds = calculateBounds(newPos + offset);
                if (!level.collides(tempBounds)) {
                    move(0.f, moved);
                    break;
                }

                tempBounds = calculateBounds(newPos - offset);
                if (!level.collides(tempBounds)) {
                    move(0.f, -moved);
                    break;
                }
            }
        }
    }

    oldPos = getPosition();

    if (MOVE_DOWN) {
        move(0.f, moved);
    }
    if (MOVE_UP) {
        move(0.f, -moved);
    }

    if (level.collides(getGlobalBounds())) {
        // Get new (collided) position
        const sf::Vector2f newPos = getPosition(); 
        
        // Restore to non-collided position
        setPosition(oldPos);

        if (!diagonal) {
            for (int i = 1; i <= 12; i++) {
                auto const offset = sf::Vector2f(i, 0.f);
                auto tempBounds = calculateBounds(newPos + offset);
                if (!level.collides(tempBounds)) {
                    move(moved, 0.f);
                    break;
                }

                tempBounds = calculateBounds(newPos - offset);
                if (!level.collides(tempBounds)) {
                    move(-moved, 0.f);
                    break;
                }
            }
        }
    }

    // Update Bounding Box
    const sf::Vector2f topLeft = getPosition() - sf::Vector2f(8.f, 8.f);
    boundingBox.setPosition(topLeft);

    // Update Window View
    view.setCenter(getPosition());
    window.setView(view);

    // Animation
    legs.play(legAnimation);

    if (!playerMoved) {
        if (legs.isLoopFrame())
            legs.stop();
    } else {
        legs.setPosition(getPosition());
        const float smoothRotation = (legs.getRotation() + legRotation) / 2;
        legs.setRotation(smoothRotation);
    }
    legs.update(sf::seconds(delta));

    // Face mouse cursor
    const sf::Vector2f& mousePos = window.mapPixelToCoords(mousePixelPos);
    sf::Vector2f cursorWRTPlayer = mousePos - getPosition();
    const float angle = std::atan2(cursorWRTPlayer.y, cursorWRTPlayer.x) * 180. / 3.1415;
    setRotation(angle);
}

void Player::drawLegs(sf::RenderWindow& target) const {
    target.draw(legs);
}


sf::FloatRect Player::getGlobalBounds() const {
    return calculateBounds(getPosition());
}

sf::FloatRect Player::calculateBounds(const sf::Vector2f& pos) {
    const sf::Vector2f topLeft = pos - sf::Vector2f(8.f, 8.f);
    
    return sf::FloatRect(
        topLeft,
        sf::Vector2f(16.f, 16.f)
    );
}

const sf::View& Player::getView() const {
    return view;
}

const sf::RectangleShape& Player::getBoundingBox() const {
    return boundingBox;
}
