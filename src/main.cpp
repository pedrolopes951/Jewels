#include "Game.hpp"

int main(int argc, char * argv[]) {
    Game game;
    if (game.init()) {
        game.run();
        game.clear();
    }

    return 0;
}
