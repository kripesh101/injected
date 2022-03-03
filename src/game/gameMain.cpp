#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

#include <common/decoration.hpp>
#include <common/tilemap.hpp>

using std::cout;

sf::RectangleShape boundingBox(sf::Vector2f(16.f, 16.f));

sf::FloatRect getPlayerBounds(sf::Sprite& player) {
    // sf::Vector2f topLeft = player.getPosition() - player.getOrigin();
    sf::Vector2f topLeft = player.getPosition() - sf::Vector2f(8.f, 8.f);

    boundingBox.setPosition(topLeft);
    
    return sf::FloatRect(
        topLeft,
        sf::Vector2f(16.f, 16.f)
    );
}

sf::Sound ak47Sound;

sf::Sprite createPlayerBullet(sf::Sprite& player, sf::Texture& bulletTexture) {
    ak47Sound.play();
    sf::Sprite bullet(bulletTexture);

    bullet.setOrigin(sf::Vector2f(bullet.getLocalBounds().width / 2, bullet.getLocalBounds().height / 2));
    bullet.setPosition(player.getPosition());
    bullet.setRotation(player.getRotation());

    return bullet;
}

int gameMain()
{
    // Game Window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED!", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    // window.setMouseCursorGrabbed(true);

    sf::View view1(sf::Vector2f(-32.f, -32.f), sf::Vector2f(300.f, 160.f));

    // Textures
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/player.png");
    playerTexture.setSmooth(false);
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/textures/bullet_sprite.png");
    bulletTexture.setSmooth(false);

    // Player Sprite
    sf::Sprite player(playerTexture);
    const sf::Vector2f offset(sf::Vector2f(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2));
    player.setOrigin(offset);
    player.setPosition(sf::Vector2f(-32.f, -32.f));

    // Sounds
    sf::SoundBuffer ak47SoundBuffer;
    ak47SoundBuffer.loadFromFile("assets/sounds/ak47-1.wav");
    ak47Sound.setBuffer(ak47SoundBuffer);

    // Loading Level Data
    int levelWidth, levelHeight, wallWidth, wallHeight, *levelData, *wallsData;
    
    if (!deserializeTileData("assets/levels/default/level.png", levelWidth, levelHeight, levelData))
        return -1;
    
    if (!deserializeTileData("assets/levels/default/wall.png", wallWidth, wallHeight, wallsData))
        return -1;

    TileMap map;
    if (!map.load("assets/textures/tileset.png", sf::Vector2u(16, 16), levelData, levelWidth, levelHeight))
        return -1;

    TileMap walls;
    if (!walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), wallsData, wallWidth, wallHeight)) 
        return -1;

    // Decorations
    std::vector<Decoration> decorations;
    std::vector<FileTexture> decorTextures;

    if (!deserializeDecorations("assets/levels/default/decor.txt", decorations, decorTextures))
        return -1;

    // Other stuff
    std::vector<sf::Sprite> bullets;
    sf::Clock clock;

    // Game Loop
    while (window.isOpen())
    {
        float delta = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Code to make player sprite face mouse cursor
            if (event.type == sf::Event::MouseMoved) {
                // Inverted formula for y axis because y axis in SFML is inverted.
                const float currentY = event.mouseMove.y - window.getSize().y / 2.;
                const float currentX = event.mouseMove.x - window.getSize().x / 2.;

                const float angle = std::atan2(currentY, currentX) * 180. / 3.1415;

                player.setRotation(angle);
            }
        }

        constexpr float fireRate = 600.f; // bullets fired per minute
        constexpr float timeBetweenShots = 60.f / fireRate;

        static float timeSinceLastShot = 0.f;
        timeSinceLastShot += delta;

        float bulletSpeed = 500.f * delta;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (timeSinceLastShot >= timeBetweenShots) {
                timeSinceLastShot = 0.f;
                sf::Sprite bullet = createPlayerBullet(player, bulletTexture);

                const float rotation = bullet.getRotation() * 3.1415f / 180.f;
                sf::Vector2f movement(cos(rotation), sin(rotation));
                movement *= 12.f;
                bullet.move(movement);

                if (!walls.collides(bullet.getGlobalBounds())) {
                    bullets.push_back(bullet);
                }
            }
        }

        for (auto it = bullets.begin(); it != bullets.end(); ++it) {
            const float rotation = it->getRotation();
            const float radRotation = rotation * 3.1415f / 180.f;
            sf::Vector2f movement(cos(radRotation), sin(radRotation));
            movement *= bulletSpeed;
            it->move(movement);

            if (walls.collides(it->getGlobalBounds())) {
                bullets.erase(it);
                it--;
            }
        }

        // Movement code

        sf::Vector2f oldPos = player.getPosition();
        bool diagonal = false;
        float speed = 100.f * delta;
        // cout << speed << "\n";
        
        // If moving diagonally, nerf speed slightly
        if (((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {
            speed *= 0.75f;
            diagonal = true;
         }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // right key is pressed: move our character
            player.move(speed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-speed, 0.f);
        }
        // If Player is in an invalid position, restore player and view to previous position
        auto playerBB = getPlayerBounds(player);
        if (walls.collides(playerBB) || decorCollides(playerBB, decorations)) {
            if (diagonal) player.setPosition(oldPos);
            else {
                player.move(0.f, 10.f);
                auto tempBB = getPlayerBounds(player);
                if (!(walls.collides(tempBB) || decorCollides(tempBB, decorations))) {
                    player.setPosition(oldPos);
                    player.move(0.f, speed);
                } else {
                    player.move(0.f, -20.f);
                    tempBB = getPlayerBounds(player);

                    player.setPosition(oldPos);

                    if (!(walls.collides(tempBB) || decorCollides(tempBB, decorations))) {
                        player.move(0.f, -speed);
                    }
                }
            }
        }
        
        oldPos = player.getPosition();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.move(0.f, -speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.move(0.f, speed);
        }
        // Do this separately for vertical and horizontal movement
        playerBB = getPlayerBounds(player);
        if (walls.collides(playerBB) || decorCollides(playerBB, decorations)) {
            if (diagonal) player.setPosition(oldPos);
            else {
                player.move(10.f, 0.f);
                auto tempBB = getPlayerBounds(player);
                if (!(walls.collides(tempBB) || decorCollides(tempBB, decorations))) {
                    player.setPosition(oldPos);
                    player.move(speed, 0.f);
                } else {
                    player.move(-20.f, 0.f);
                    tempBB = getPlayerBounds(player);

                    player.setPosition(oldPos);

                    if (!(walls.collides(tempBB) || decorCollides(tempBB, decorations))) {
                        player.move(-speed, 0.f);
                    }
                }
            }
        }

        view1.setCenter(player.getPosition());

        window.setView(view1);

        static float kekw = 0.0f;
        kekw += delta * 0.3f;
        const float r = std::abs(sin(kekw)) * 64;
        const float b = std::abs(cos(kekw + 1)) * 96;

        window.clear(sf::Color(32 + r, 0, 32 + b, 0));
         
        window.draw(map);
        window.draw(walls);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)) {
            getPlayerBounds(player);
            window.draw(boundingBox);
        }

        for (const auto& decor : decorations) {
            window.draw(decor);
        }

        for (const auto& bullet : bullets)
            window.draw(bullet);

        window.draw(player);

        window.display();
    }

    delete wallsData, levelData;

    return 0;
}
