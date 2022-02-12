#include <algorithm>
#include <string>

int gameMain();
int editorMain(const std::string& targetFolder);

int main(int argc, char *argv[])
{
    if (argc >= 2)
        if (std::find(argv, argv + argc, "-editor")) {
            if (argc == 2)
                return editorMain("assets/levels/default/");
            else
                return editorMain(argv[2]);
        }
    
    return gameMain();
}
