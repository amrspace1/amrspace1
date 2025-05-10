#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Resources.h"

struct Position {
    float x;
    float y;
};

class Bike {
private:
    float x;  // Horizontal position relative to road center
    float y;  // Vertical position
    float screenX; // Actual screen X position
    float velX;
    float velY;
    bool isJumping;
    int width;
    int height;
    Resources* resources;
    float forwardSpeed;
    float acceleration;
    float airControl;  // Amount of horizontal control while in air (0-1)

public:
    Bike(float y, Resources* resources);
    ~Bike();
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void handleKeyPress(SDL_Keycode key);
    void handleKeyRelease(SDL_Keycode key);
    void jump();
    
    SDL_Rect getCollisionBox() const;
    Position getPosition() const;
    float getX() const;
    float getY() const;
    float getForwardSpeed() const;
};
