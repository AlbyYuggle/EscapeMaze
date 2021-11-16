#include "game.hpp"

using namespace std;

int main()
{
    Level level{0.01F, 0.0125F, 0.0125F, 0, 0.0F, 20, 20, {-0.6625F, 0.6625F}, {0.6625F, -0.6625F}};
    Game *game = new Game();
    game->AddLevel(level);
    game->Init();
    game->GenerateNextLevel();
    return 0;
}
