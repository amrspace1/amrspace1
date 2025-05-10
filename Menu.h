#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <SDL2/SDL_image.h>
#include "Resources.h"

class Menu {
private:
    SDL_Renderer* renderer;
    Resources* resources;
    std::vector<std::string> options;
    int selectedOption;
    
public:
    Menu(SDL_Renderer* renderer, Resources* resources);
    ~Menu();
    
    void handleKeyPress(SDL_Keycode key);
    void update();
    void render();
    int getSelectedOption() const;
};