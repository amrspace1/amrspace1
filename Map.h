#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Obstacle.h"
#include "Resources.h"

class Map {
private:
    SDL_Renderer* renderer;
    Resources* resources;
    std::vector<Obstacle*> obstacles;
    float finishLineX;
    float cameraX;
    float lastObstacleX;
    float timeSinceLastObstacle;
    
    void generateInitialObstacles();
    void createRandomObstacle(float x);//
    int getRandom(int min, int max);
    
public:
    Map(SDL_Renderer* renderer, Resources* resources);
    ~Map();
    
    void update(float bikeSpeed, float deltaTime);
    void render();
    bool checkCollision(const SDL_Rect& rect, float bikeX);
    bool isFinished(float bikeX);
    
    float getCameraX() const;
};