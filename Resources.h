#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

class Resources {
private:
    SDL_Renderer* renderer;
    std::map<std::string, SDL_Texture*> textures;
    
public:
    Resources(SDL_Renderer* renderer);
    ~Resources();
    
    bool loadTexture(const std::string& id, const std::string& path);
    bool loadAll();
    SDL_Texture* getTexture(const std::string& id) const;
};