#include "Menu.h"
#include <SDL2/SDL_ttf.h>
#include "Constants.h"

Menu::Menu(SDL_Renderer* renderer, Resources* resources)
    : renderer(renderer), resources(resources), selectedOption(0) {
    options = {"JOUER", "RESTART", "A PROPOS", "QUITTER"};
}

Menu::~Menu() {}

void Menu::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP: selectedOption = (selectedOption - 1 + options.size()) % options.size(); break;
        case SDLK_DOWN: selectedOption = (selectedOption + 1) % options.size(); break;
    }
}

void Menu::update() {}

void Menu::render() {
    // Render background
    SDL_Texture* bg = resources->getTexture("menu_background");
    if (bg) {
        SDL_RenderCopy(renderer, bg, NULL, NULL);
    } else {
        // Dark blue background as fallback
        SDL_SetRenderDrawColor(renderer, 25, 25, 75, 255);
        SDL_RenderClear(renderer);
    }
    
    // Menu is now positioned on the left without a special background panel
    int menuLeftMargin = 50; // Left margin for the menu options
    int menuWidth = 250;     // Width of menu option boxes
    
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 28);
    if (!font) return;
    
    // Title - Centered on screen
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "BIKE RACE", white);
    if (titleSurface) {
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        if (titleTexture) {
            // Center the title in the entire screen
            SDL_Rect titleRect = {
                (SCREEN_WIDTH - titleSurface->w)/2, 
                50, 
                titleSurface->w, 
                titleSurface->h
            };
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
            SDL_DestroyTexture(titleTexture);
        }
        SDL_FreeSurface(titleSurface);
    }
    
    // Options - with borders
    int optionStartY = 200;
    int optionHeight = 50;
    int optionSpacing = optionHeight + 20; // Space between options
    
    for (size_t i = 0; i < options.size(); i++) {
        // Option box position
        int y = optionStartY + static_cast<int>(i) * optionSpacing;
        
        // Menu item background & border
        SDL_Rect optionBox = {
            menuLeftMargin,
            y,
            menuWidth,
            optionHeight
        };
        
        // Selected option gets a different background
        if (i == selectedOption) {
            // Highlighted background for selected option
            SDL_SetRenderDrawColor(renderer, 100, 100, 180, 255);
            SDL_RenderFillRect(renderer, &optionBox);
            
            // Gold border for selected option
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
        } else {
            // Normal background for unselected options
            SDL_SetRenderDrawColor(renderer, 70, 70, 120, 255);
            SDL_RenderFillRect(renderer, &optionBox);
            
            // White border for unselected options
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        }
        
        // Draw the border
        SDL_RenderDrawRect(renderer, &optionBox);
        
        // Render the text
        SDL_Color textColor = (i == selectedOption) ? SDL_Color{255, 255, 0, 255} : white;
        SDL_Surface* optionSurface = TTF_RenderText_Solid(font, options[i].c_str(), textColor);
        if (optionSurface) {
            SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurface);
            if (optionTexture) {
                // Center text inside the option box
                SDL_Rect textRect = {
                    optionBox.x + (optionBox.w - optionSurface->w) / 2,
                    optionBox.y + (optionBox.h - optionSurface->h) / 2,
                    optionSurface->w,
                    optionSurface->h
                };
                SDL_RenderCopy(renderer, optionTexture, NULL, &textRect);
                SDL_DestroyTexture(optionTexture);
            }
            SDL_FreeSurface(optionSurface);
        }
    }
    
    TTF_CloseFont(font);
}

int Menu::getSelectedOption() const {
    return selectedOption;
}