#pragma once
#include <SDL2/SDL.h>

class CollisionDetection {
public:
    static bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    static bool checkPointCollision(int x, int y, const SDL_Rect& rect);
};