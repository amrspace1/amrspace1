#include "Resources.h"
#include <iostream>

Resources::Resources(SDL_Renderer* renderer) : renderer(renderer) {}

Resources::~Resources() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
}

bool Resources::loadTexture(const std::string& id, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return false;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture: " << path << " - " << SDL_GetError() << std::endl;
        return false;
    }
    
    textures[id] = texture;
    return true;
}
bool Resources::loadAll() {
    bool success = true;
    
    // Main game assets
    success &= loadTexture("bike", "assets/images/bike.png");
    
    // Obstacles
    success &= loadTexture("wall", "assets/images/walle.png");
    success &= loadTexture("ramp", "assets/images/ramp.png");
    success &= loadTexture("pit", "assets/images/pit.png");
    
    // Road and menu bike race
    success &= loadTexture("road", "assets/images/road.png");
    success &= loadTexture("menu_background", "assets/images/menu_background.png");
    
    // Background layers (distant -> close)
    success &= loadTexture("bg_layer0", "assets/images/bg_layer0.png"); // السماء البعيدة
    success &= loadTexture("bg_layer1", "assets/images/bg_layer1.png"); // الجبال
    success &= loadTexture("bg_layer2", "assets/images/bg_layer2.png");    // Close trees
    
    return success;
}

SDL_Texture* Resources::getTexture(const std::string& id) const {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}