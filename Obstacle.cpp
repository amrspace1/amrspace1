#include "Obstacle.h"
#include "Resources.h"

Obstacle::Obstacle(float x, float y, int width, int height, ObstacleType type, Resources* resources)
    : x(x), y(y), width(width), height(height), type(type), resources(resources) {}

Obstacle::~Obstacle() {}

// Obstacle.cpp
void Obstacle::render(SDL_Renderer* renderer, float cameraX) {
    SDL_Rect destRect = {
        static_cast<int>(x - cameraX),
        static_cast<int>(y),
        width,
        height
    };
    
    // use same off the obstacle
    const char* textureName = "";
    switch(type) {
        case WALL: textureName = "wall"; break;
        case RAMP: textureName = "ramp"; break;
        case PIT: textureName = "pit"; break;
    }
    
    SDL_Texture* texture = resources->getTexture(textureName);
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    } else {
        // change draw if the reseaurce inuploaded// matanstalax 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}

SDL_Rect Obstacle::getCollisionBox(float cameraX) const {
    return {(int)(x - cameraX), (int)y, width, height};
}

ObstacleType Obstacle::getType() const {
    return type;
}