#include "Map.h"
#include "Resources.h"
#include <cstdlib>
#include <algorithm>
#include <ctime>

Map::Map(SDL_Renderer* renderer, Resources* resources)
    : renderer(renderer), resources(resources), finishLineX(5000),
      cameraX(0), lastObstacleX(800), timeSinceLastObstacle(0) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    generateInitialObstacles();
}

Map::~Map() {
    for (auto obstacle : obstacles) {
        delete obstacle;
    }
}

void Map::generateInitialObstacles() {
    float obstacleX = 800;
    for (int i = 0; i < 5; i++) {
        createRandomObstacle(obstacleX);
        obstacleX += getRandom(400, 600);
    }
}
void Map::createRandomObstacle(float x) {
    // تحديد موقع العائق على الطريق
    float obstacleY = ROAD_Y_POSITION - 60; // 50 هو ارتفاع العائق
    
    // تحديد نوع العائق
    int obstacleType = rand() % 3;
    
    obstacles.push_back(new Obstacle(
        x, 
        obstacleY,  // موقع رأسي ثابت على الطريق
        80, 80,     // أبعاد العائق
        static_cast<ObstacleType>(obstacleType), 
        resources
    ));
}

void Map::update(float bikeSpeed, float deltaTime) {
    cameraX += bikeSpeed * deltaTime;
    
    // توليد عوائق جديدة أمام الدراجة
    float lastObstacleX = obstacles.empty() ? cameraX : obstacles.back()->getX();
    if (lastObstacleX < cameraX + SCREEN_WIDTH * 2) {
        createRandomObstacle(lastObstacleX + getRandom(400, 700));
    }
    
    // إزالة العوائق خلف الدراجة
    auto it = obstacles.begin();
    while (it != obstacles.end()) {
        if ((*it)->getX() < cameraX - 100) {
            delete *it;
            it = obstacles.erase(it);
        } else {
            ++it;
        }
    }
}

int Map::getRandom(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}



void Map::render() {
    // Background layers
    SDL_Texture* bgSky = resources->getTexture("bg_layer0");
    SDL_Texture* bgMountains = resources->getTexture("bg_layer1");
    SDL_Texture* bgTrees = resources->getTexture("bg_layer2");
    
    if (bgSky && bgMountains && bgTrees) {
        int skyOffset = static_cast<int>(cameraX * 0.2f) % SCREEN_WIDTH;
        SDL_Rect skyRect1 = {-skyOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgSky, NULL, &skyRect1);
        SDL_Rect skyRect2 = {SCREEN_WIDTH - skyOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgSky, NULL, &skyRect2);
        
        int mountainsOffset = static_cast<int>(cameraX * 0.5f) % SCREEN_WIDTH;
        SDL_Rect mountainsRect1 = {-mountainsOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgMountains, NULL, &mountainsRect1);
        SDL_Rect mountainsRect2 = {SCREEN_WIDTH - mountainsOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgMountains, NULL, &mountainsRect2);
        
        int treesOffset = static_cast<int>(cameraX * 0.8f) % SCREEN_WIDTH;
        SDL_Rect treesRect1 = {-treesOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgTrees, NULL, &treesRect1);
        SDL_Rect treesRect2 = {SCREEN_WIDTH - treesOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgTrees, NULL, &treesRect2);
    } else {
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);
    }

    // Road
    SDL_Texture* roadTexture = resources->getTexture("road");
    if (roadTexture) {
        int roadOffset = static_cast<int>(cameraX) % SCREEN_WIDTH;
        SDL_Rect roadRect1 = {-roadOffset, ROAD_Y_POSITION, SCREEN_WIDTH, ROAD_HEIGHT};
        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect1);
        SDL_Rect roadRect2 = {SCREEN_WIDTH - roadOffset, ROAD_Y_POSITION, SCREEN_WIDTH, ROAD_HEIGHT};
        SDL_RenderCopy(renderer, roadTexture, NULL, &roadRect2);
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_Rect groundRect = {0, ROAD_Y_POSITION, SCREEN_WIDTH, ROAD_HEIGHT};
        SDL_RenderFillRect(renderer, &groundRect);
    }

    // Obstacles
    for (auto obstacle : obstacles) {
        obstacle->render(renderer, cameraX);
    }

    // Finish line// seul condition off win the game 
    float finishScreenPos = finishLineX - cameraX;
    if (finishScreenPos >= 0 && finishScreenPos < SCREEN_WIDTH) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect finishRect = {static_cast<int>(finishScreenPos), ROAD_Y_POSITION, 40, ROAD_HEIGHT};
        SDL_RenderFillRect(renderer, &finishRect);
    }
}

bool Map::checkCollision(const SDL_Rect& bikeRect, float bikeX) {
    for (auto obstacle : obstacles) {
        SDL_Rect obstacleRect = obstacle->getCollisionBox(cameraX);
        if (SDL_HasIntersection(&bikeRect, &obstacleRect)) {
            return true;
        }
    }
    return false;
}

bool Map::isFinished(float bikeX) {
    return bikeX >= finishLineX;
}

float Map::getCameraX() const {
    return cameraX;  // Fixed typo here
}