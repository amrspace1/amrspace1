 #pragma once
 #include <SDL2/SDL.h>
 #include <vector>
 #include <stdexcept>
 #include <SDL2/SDL_image.h>
 #include "Resources.h"
 #include "Constants.h"

class Background {
    private:
        SDL_Renderer* renderer;
        Resources* resources;
        std::vector<SDL_Texture*> layers;
        std::vector<float> offsets;
        std::vector<float> PARALLAX_SPEEDS;
        SDL_Texture* roadTexture;
        float roadOffset;
        float roadSpeed;
        
        
        static const int BG_LAYERS = 3;
        static const int SCREEN_WIDTH = 1280; // Adjust as needed
        static const int SCREEN_HEIGHT = 720; // Adjust as needed
        
    public:
        Background(SDL_Renderer* renderer, Resources* resources);
        ~Background();
        void update(float cameraX);
        void render();
        void renderRoad();
    };


    
// // class Background {
// // private:
// //     SDL_Renderer* renderer;
// //     Resources* resources;
// //     std::vector<SDL_Texture*> layers;
// //     std::vector<float> offsets;
    
// // public:
// //     Background(SDL_Renderer* renderer, Resources* resources);
// //     ~Background();
// //     void update(float cameraX);
// //     void render();
// // };