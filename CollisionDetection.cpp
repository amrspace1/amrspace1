
#include "CollisionDetection.h"

bool CollisionDetection::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w) &&
           (a.x + a.w > b.x) &&
           (a.y < b.y + b.h) &&
           (a.y + a.h > b.y);
}
bool CollisionDetection::checkPointCollision(int x, int y, const SDL_Rect& rect) {
    // Vérifier si un point est à l'intérieur d'un rectangle
    return (x >= rect.x && x < rect.x + rect.w &&
            y >= rect.y && y < rect.y + rect.h);
}