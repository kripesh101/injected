#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

enum EditorMode {
    NONE,
    LEVEL_TILE,
    WALL_TILE
};

class EditorHelper {
public:
    void processInputs(const sf::Event& event);
    void setStatus(EditorMode mode, int tileNum);
    bool isUserEntering() const;
    int getCurrentTileNumber() const;
    EditorMode getCurrentMode() const;
    void updateText(sf::Text& text) const;
private:
    EditorMode currentMode = NONE;
    EditorMode nextMode = NONE;
    int currentTileNum = 0;
    int nextTileNum = 0;
    bool userEntering = false;

    static void textHelper(sf::String& str, EditorMode mode, int tileNum);
};
