#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using std::cout;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
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

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 640), "INJECTED!", sf::Style::Default, settings);
    window.setMouseCursorGrabbed(true);

    sf::View view1(sf::Vector2f(16.f, 16.f), sf::Vector2f(300.f, 160.f));

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/textures/player.png");
    playerTexture.setSmooth(false);

    sf::Sprite player(playerTexture);
    const sf::Vector2f offset(sf::Vector2f(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2));
    player.setOrigin(offset);
    player.setPosition(sf::Vector2f(16.f, 16.f));

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

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("assets/textures/tileset.png", sf::Vector2u(16, 16), level, 16, 8))
        return -1;


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

        // Movement code
        float speed = 60.f * delta;
        
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
        window.setView(view1);


        static float kekw = 0.0f;
        kekw += delta * 0.3f;
        const float r = std::abs(sin(kekw)) * 64;
        const float b = std::abs(cos(kekw + 1)) * 96;

        window.clear(sf::Color(32 + r, 0, 32 + b, 0));
        
        window.draw(map);
        window.draw(player);
        window.display();
    }

    return 0;
}
