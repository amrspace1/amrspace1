#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;
    
    try {
        if (!game.init()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }
        
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

