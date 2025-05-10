#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Resources.h"
#include "Constants.h"

class Obstacle {
private:
    float x, y;
    int width, height;
    ObstacleType type;
    Resources* resources;
    
public:
    Obstacle(float x, float y, int width, int height, ObstacleType type, Resources* resources);
    ~Obstacle();
    float getY() const { return y; }
    void setY(float newY) { y = newY; }
    void render(SDL_Renderer* renderer, float cameraX);
    SDL_Rect getCollisionBox(float cameraX) const;
    ObstacleType getType() const;
    float getX() const { return x; }
};