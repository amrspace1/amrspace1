#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "Resources.h"
#include "Background.h"
#include "Menu.h"
#include "Map.h"
#include "Bike.h"
#include "Timer.h"

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    ABOUT
};

class GameException : public std::exception {
private:
    std::string message;
    
public:
    GameException(const std::string& msg) : message(msg) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class Game {
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Bike* playerBike;
    Map* gameMap;
    Menu* mainMenu;
    Timer* gameTimer;
    Resources* gameResources;
    Background* background;
    
    Uint32 lastFrameTime;
    float deltaTime;
    
    GameState currentState;
    
public:
    Game();
    ~Game();
    
    bool init();
    void initGameObjects();
    void restartGame();
    void handleEvents();
    void update();
    void render();
    void clean();
    void run();
};