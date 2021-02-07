#include <string>

#include "Game.hpp"

int main(int argc, char** argv) {
  
    if (argc == 2) {
        if (strcmp(argv[1], "p") == 0) {
            mygame::Paddle::NUM_OF_PADDLE = 2;
        }
    }
    mygame::Game game;
    bool success = game.Initialize();
    if (success) {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}