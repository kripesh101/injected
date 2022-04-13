#include <algorithm>
#include <string>
#include <vector>

int gameMain();
int editorMain(const std::string& targetFolder);

bool DEBUG = false;

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        std::vector <std::string> tokens;
        for (int i = 1; i < argc; i++)
            tokens.push_back(std::string(argv[i]));

        if (std::find(tokens.begin(), tokens.end(), "-editor") != tokens.end()) {
            if (argc == 2) {
                return editorMain("assets/editor_default_level/");
            } else {
                return editorMain(*(tokens.begin() + 1));
            }
        }
        else if (std::find(tokens.begin(), tokens.end(), "-debug") != tokens.end()) {
            DEBUG = true;
        }
    }
    
    return gameMain();
}
