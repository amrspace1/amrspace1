#include "Bike.h"
#include "Constants.h"
#include <cmath>

Bike::Bike(float y, Resources* resources) : 
    x(0), // Centered on road
    y(y), 
    screenX(ROAD_LEFT_MARGIN + (ROAD_WIDTH / 2) - (width / 2)),
    velX(0), velY(0), 
    isJumping(false), 
    width(80), height(60),
    resources(resources), 
    forwardSpeed(BIKE_FORWARD_SPEED),
    acceleration(0),
    airControl(0.5f) {}

Bike::~Bike() {
}

void Bike::update(float deltaTime) {
    // Apply acceleration to forward speed (smoother speed changes)
    forwardSpeed += acceleration * deltaTime;
    
    // Cap max speed
    if (forwardSpeed > MAX_BIKE_SPEED) {
        forwardSpeed = MAX_BIKE_SPEED;
    } else if (forwardSpeed < MIN_BIKE_SPEED) {
        forwardSpeed = MIN_BIKE_SPEED;
    }
    
    // Horizontal movement
    x += velX * deltaTime;
    
    // Keep bike within road bounds
    float maxOffset = ROAD_WIDTH / 2 - width / 2;
    if (x < -maxOffset) x = -maxOffset;
    if (x > maxOffset) x = maxOffset;
    
    // Update actual screen position
    screenX = ROAD_LEFT_MARGIN + (ROAD_WIDTH / 2) - (width / 2) + x;
    
    // Vertical movement (jumping)
    if (isJumping) {
        // Apply gravity
        velY += GRAVITY * deltaTime;
        y += velY * deltaTime;
        
        // Land on ground
        if (y >= ROAD_Y_POSITION) {
            y = ROAD_Y_POSITION;
            isJumping = false;
            velY = 0;
            
            // Landing effect - slight speed decrease
            forwardSpeed *= 0.9f;
        }
        
        // Apply reduced horizontal control while in air
        if (velX != 0) {
            x += velX * airControl * deltaTime;
        }
    }
}

void Bike::render(SDL_Renderer* renderer) {
    SDL_Texture* bikeTexture = resources->getTexture("bike");
    if (bikeTexture) {
        SDL_Rect destRect = {(int)screenX, (int)y, width, height};
        
        // Adjust angle based on vertical velocity (bike tilts during jumps)
        double angle = 0;
        if (isJumping) {
            // Calculate angle based on velocity
            angle = -velY * 0.1f; // Tilt forward when going up, backward when falling
            angle = std::max(-30.0, std::min(30.0, (double)angle)); // Limit angle
        }
        
        // Render with rotation
        SDL_RenderCopyEx(renderer, bikeTexture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
    } else {
        // Fallback rendering if texture not found
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect bikeRect = {(int)screenX, (int)y, width, height};
        SDL_RenderFillRect(renderer, &bikeRect);
    }
}

void Bike::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_LEFT:
            velX = -BIKE_SPEED;
            break;
            
        case SDLK_RIGHT:
            velX = BIKE_SPEED;
            break;
            
        case SDLK_UP:
        case SDLK_SPACE:
            if (!isJumping) {
                jump();
            }
            break;
            
        // Add acceleration controls
        case SDLK_w:
            acceleration = BIKE_ACCELERATION;
            break;
            
        case SDLK_s:
            acceleration = -BIKE_ACCELERATION;
            break;
    }
}

void Bike::handleKeyRelease(SDL_Keycode key) {
    switch (key) {
        case SDLK_LEFT: 
            if (velX < 0) velX = 0; 
            break;
            
        case SDLK_RIGHT: 
            if (velX > 0) velX = 0; 
            break;
            
        case SDLK_UP:
        case SDLK_SPACE:
            // Reduce jump height if key released early (variable jump height)
            if (isJumping && velY < -5.0f) {
                velY = -5.0f;
            }
            break;
            
        // Stop acceleration when keys released
        case SDLK_w:
        case SDLK_s:
            acceleration = 0;
            break;
    }
}

void Bike::jump() {
    if (!isJumping) {
        velY = -BIKE_JUMP_FORCE;
        isJumping = true;
        
        // Small forward boost when jumping
        forwardSpeed += 50.0f;
        if (forwardSpeed > MAX_BIKE_SPEED) {
            forwardSpeed = MAX_BIKE_SPEED;
        }
    }
}

SDL_Rect Bike::getCollisionBox() const {
    return {(int)screenX, (int)y, width, height};
}

Position Bike::getPosition() const {
    return {x, y};
}

float Bike::getX() const {
    return x;
}

float Bike::getY() const {
    return y;
}

float Bike::getForwardSpeed() const {
    return forwardSpeed;
}