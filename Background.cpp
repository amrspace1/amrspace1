#include "Background.h" 
#include <stdexcept>

Background::Background(SDL_Renderer* renderer, Resources* resources) 
    : renderer(renderer), resources(resources) {
    
    // Validate inputs
    if (!renderer) {
        throw std::invalid_argument("Renderer cannot be null");
    }
    if (!resources) {
        throw std::invalid_argument("Resources cannot be null");
    }

    // Load textures with error checking
    try {
        layers.push_back(resources->getTexture("bg_layer0"));
        layers.push_back(resources->getTexture("bg_layer1"));
        layers.push_back(resources->getTexture("bg_layer2"));
        
        // Verify all layers loaded
        for (auto& layer : layers) {
            if (!layer) {
                throw std::runtime_error("Failed to load background texture");
            }
        }
        
        offsets.resize(BG_LAYERS, 0);
        
        // Initialize parallax speeds (add to header if needed)
        PARALLAX_SPEEDS = {0.1f, 0.3f, 0.6f}; // Slower speeds for distant layers

    // Load road texture with better blending
    roadTexture = resources->getTexture("road");
    if (!roadTexture) {
        throw std::runtime_error("Failed to load road texture");
    }
    
    roadOffset = 0;
    roadSpeed = 1.2f;  // Road moves slightly faster than camera
        
    } catch (const std::exception& e) {
        // Cleanup if initialization fails
        layers.clear();
        throw; // Re-throw the exception
    }
}

Background::~Background() {
    // Textures are owned by Resources, no need to destroy them here
    layers.clear();
}

void Background::update(float cameraX) {
    // Validate we have proper layers
    if (offsets.size() != BG_LAYERS || layers.size() != BG_LAYERS) {
        return;
    }
    
    for (int i = 0; i < BG_LAYERS; i++) {
        // Ensure we don't access out of bounds
        if (i < PARALLAX_SPEEDS.size()) {
            offsets[i] = cameraX * PARALLAX_SPEEDS[i];
        }
    }
    
    // Update road offset with proper speed
    roadOffset = cameraX * roadSpeed;
}
void Background::render() {
    if (!renderer) return;
    
    for (int i = 0; i < BG_LAYERS && i < layers.size(); i++) {
        if (layers[i]) {
            // Calculate positions
            int x = -static_cast<int>(offsets[i]) % SCREEN_WIDTH;
            
            // First copy
            SDL_Rect dst1 = {x, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, layers[i], nullptr, &dst1);
            
            // Second copy (for seamless scrolling)
            SDL_Rect dst2 = {x + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, layers[i], nullptr, &dst2);
        }
    }
}
void Background::renderRoad() {
    if (!renderer || !roadTexture) return;
    
    // Render road with shadow effect
    SDL_SetTextureBlendMode(roadTexture, SDL_BLENDMODE_BLEND);
    
    int x = -static_cast<int>(roadOffset) % SCREEN_WIDTH;
    
    // First copy with shadow
    SDL_SetTextureAlphaMod(roadTexture, 200);
    SDL_Rect dst1 = {x, ROAD_Y_POSITION + 5, SCREEN_WIDTH, ROAD_HEIGHT};
    SDL_RenderCopy(renderer, roadTexture, nullptr, &dst1);
    
    // Second copy with shadow
    SDL_Rect dst2 = {x + SCREEN_WIDTH, ROAD_Y_POSITION + 5, SCREEN_WIDTH, ROAD_HEIGHT};
    SDL_RenderCopy(renderer, roadTexture, nullptr, &dst2);
    
    // Main road
    SDL_SetTextureAlphaMod(roadTexture, 255);
    dst1.y = ROAD_Y_POSITION;
    dst2.y = ROAD_Y_POSITION;
    SDL_RenderCopy(renderer, roadTexture, nullptr, &dst1);
    SDL_RenderCopy(renderer, roadTexture, nullptr, &dst2);
}