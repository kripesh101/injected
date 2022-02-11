#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

using std::cout;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tilesize2, const int* tilesData, unsigned int width, unsigned int height)
    {
        tileSize = tilesize2;
        tilemapSize = sf::Vector2u(width, height);
        tiles = tilesData;

        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;
        m_tileset.setSmooth(false);

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x + 0.1f, tv * tileSize.y + 0.1f);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x - 0.1f, tv * tileSize.y + 0.1f);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x - 0.1f, (tv + 1) * tileSize.y - 0.1f);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x + 0.1f, (tv + 1) * tileSize.y - 0.1f);
            }

        return true;
    }

    bool collides(const sf::FloatRect& bb) {
        // cout << "\n";
        return pointCollides(bb.left, bb.top) || pointCollides(bb.left + 8, bb.top) 
            || pointCollides(bb.left + bb.width, bb.top) || pointCollides(bb.left + bb.width, bb.top + 8) 
            || pointCollides(bb.left + bb.width, bb.top + bb.height) || pointCollides(bb.left + 8, bb.top + bb.height)
            || pointCollides(bb.left, bb.top + bb.height) || pointCollides(bb.left, bb.top + 8);
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    bool pointCollides(const float& x, const float& y) const {
        const sf::Vector2i tile = getTileCoordinates(x, y);
        
        // cout << tile.x << " " << tile.y << " | ";

        // Tile coordinates outside of tilemap
        if (tile.x < 0 || tile.y < 0 || tile.x >= tilemapSize.x || tile.y >= tilemapSize.y)
            return false;

        // If tile data is 0, no collision.
        // Else collide.
        if (tiles[tile.x + tile.y * tilemapSize.x] == 0) return false;
        else return true;
    }

    sf::Vector2i getTileCoordinates(float x, float y) const {
        // const sf::Vector2f& tilemapPos = getPosition();
        // x -= tilemapPos.x;
        // y -= tilemapPos.y;

        return sf::Vector2i(floor(x / tileSize.x), floor(y / tileSize.y));
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u tileSize;
    sf::Vector2u tilemapSize;

    const int* tiles;
};

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
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED!", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    // window.setMouseCursorGrabbed(true);

    sf::View view1(sf::Vector2f(32.f, 32.f), sf::Vector2f(300.f, 160.f));

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/player.png");
    playerTexture.setSmooth(false);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/textures/bullet_sprite.png");
    bulletTexture.setSmooth(false);

    sf::Sprite player(playerTexture);
    const sf::Vector2f offset(sf::Vector2f(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2));
    player.setOrigin(offset);
    player.setPosition(sf::Vector2f(32.f, 32.f));

    sf::SoundBuffer ak47SoundBuffer;
    ak47SoundBuffer.loadFromFile("assets/sounds/ak47-1.wav");
    ak47Sound.setBuffer(ak47SoundBuffer);

    sf::Clock clock;


    // define the level with an array of tile indices
    const int level[] =
    {
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    };

    const int levelwalls[] = {
        1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
        1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    
    };

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("assets/textures/tileset.png", sf::Vector2u(16, 16), level, 16, 8))
        return -1;
    // map.move(32, 32);

    TileMap walls;
    if (!walls.load("assets/textures/wall_tileset.png", sf::Vector2u(8, 8), levelwalls, 32, 16)) 
        return -1;

    std::vector<sf::Sprite> bullets;

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

        sf::Vector2f oldPos = player.getPosition();
        sf::View oldView = view1;

        // Movement code
        float speed = 100.f * delta;
        // cout << speed << "\n";
        
        // If moving diagonally, nerf speed slightly
        if (((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A))) 
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S))))
            speed *= 0.75f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // right key is pressed: move our character
            player.move(speed, 0.f);
            view1.move(speed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-speed, 0.f);
            view1.move(-speed, 0.f);
        }
        // If Player is in an invalid position, restore player and view to previous position
        if (walls.collides(getPlayerBounds(player))) {
            player.setPosition(oldPos);
            view1 = oldView;
        }
        
        oldPos = player.getPosition();
        oldView = view1;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.move(0.f, -speed);
            view1.move(0.f, -speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.move(0.f, speed);
            view1.move(0.f, speed);
        }
        // Do this separately for vertical and horizontal movement
        if (walls.collides(getPlayerBounds(player))) {
            player.setPosition(oldPos);
            view1 = oldView;
        }

        window.setView(view1);

        static float kekw = 0.0f;
        kekw += delta * 0.3f;
        const float r = std::abs(sin(kekw)) * 64;
        const float b = std::abs(cos(kekw + 1)) * 96;

        window.clear(sf::Color(32 + r, 0, 32 + b, 0));
         
        window.draw(map);
        window.draw(walls);
        // window.draw(boundingBox);


        for (auto bullet : bullets)
            window.draw(bullet);

        window.draw(player);

        window.display();
    }

    return 0;
}
