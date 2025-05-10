#include "Game.h"
#include "Constants.h"
#include "Bike.h"
#include <iostream>

Game::Game() 
    : isRunning(false), window(nullptr), renderer(nullptr),
      playerBike(nullptr), gameMap(nullptr), mainMenu(nullptr),
      gameTimer(nullptr), gameResources(nullptr), background(nullptr),
      lastFrameTime(0), deltaTime(0), currentState(MENU) {}

Game::~Game() {
    clean();
}

bool Game::init() {
    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            throw GameException("SDL init failed: " + std::string(SDL_GetError()));
        }
        
        if (TTF_Init() == -1) {
            throw GameException("TTF init failed: " + std::string(TTF_GetError()));
        }
        
        window = SDL_CreateWindow("Bike Race", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            throw GameException("Window creation failed: " + std::string(SDL_GetError()));
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            throw GameException("Renderer creation failed: " + std::string(SDL_GetError()));
        }
        
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            throw GameException("SDL_image init failed: " + std::string(IMG_GetError()));
        }
        
        gameResources = new Resources(renderer);
        if (!gameResources->loadAll()) {
            throw GameException("Failed to load resources");
        }
        
        initGameObjects();
        
        lastFrameTime = SDL_GetTicks();
        isRunning = true;
        return true;
        
    } catch (const GameException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

void Game::initGameObjects() {
    // Create new game objects
    background = new Background(renderer, gameResources);
    playerBike = new Bike(ROAD_Y_POSITION, gameResources);
    gameMap = new Map(renderer, gameResources);
    mainMenu = new Menu(renderer, gameResources);
    gameTimer = new Timer(GAME_TIME_LIMIT);
}

void Game::restartGame() {
    // Delete existing objects
    delete playerBike;
    delete gameMap;
    delete gameTimer;
    
    // Create new objects
    playerBike = new Bike(ROAD_Y_POSITION, gameResources);
    gameMap = new Map(renderer, gameResources);
    gameTimer = new Timer(GAME_TIME_LIMIT);
    
    // Start the game
    currentState = PLAYING;
    gameTimer->start();
}

void Game::handleEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (currentState) {
                    case MENU:
                        mainMenu->handleKeyPress(event.key.keysym.sym);
                        if (event.key.keysym.sym == SDLK_RETURN) {
                            switch (mainMenu->getSelectedOption()) {
                                case 0: // PLAY
                                    currentState = PLAYING;
                                    gameTimer->start();
                                    break;
                                case 1: // RESTART
                                    restartGame();
                                    break;
                                case 2: // ABOUT
                                    currentState = ABOUT;
                                    break;
                                case 3: // QUIT
                                    isRunning = false;
                                    break;
                            }
                        }
                        break;
                    
                    case PLAYING:
                        playerBike->handleKeyPress(event.key.keysym.sym);
                        // Pause game with ESC
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = MENU;
                        }
                        // Restart with R key
                        if (event.key.keysym.sym == SDLK_r) {
                            restartGame();
                        }
                        break;
                    
                    case GAME_OVER:
                        // Restart with R key from game over screen
                        if (event.key.keysym.sym == SDLK_r) {
                            restartGame();
                        }
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = MENU;
                        }
                        break;

                    case ABOUT:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = MENU;
                        }
                        break;
                }
                break;
            
            case SDL_KEYUP:
                if (currentState == PLAYING) {
                    // Call the correct function for key release
                    playerBike->handleKeyRelease(event.key.keysym.sym);
                }
                break;
        }
    }
}

void Game::update() {
    // Calculate delta time for smooth movement
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastFrameTime) / 1000.0f;
    lastFrameTime = currentTime;
    
    // Cap delta time to prevent large jumps in physics
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    
    if (currentState != PLAYING) return;
    
   
    playerBike->update(deltaTime);
    
    // Update map with bike's 
    gameMap->update(playerBike->getForwardSpeed(), deltaTime);
    
    // Check collisions
    SDL_Rect bikeRect = playerBike->getCollisionBox();
    if (gameMap->checkCollision(bikeRect, playerBike->getX())) {
        currentState = GAME_OVER;
    }
    
    // Check if player has reached finish line
    if (gameMap->isFinished(playerBike->getX())) {
        currentState = GAME_OVER; // Game over but with victory state
    }
    
    // Update timer
    gameTimer->update();
    if (gameTimer->isTimeOver()) {
        currentState = GAME_OVER;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
        case PLAYING:
            // Render in correct order
            background->render();      // Distant background
            gameMap->render();         // Ground and obstacles
            background->renderRoad();  // Road on top of ground
            playerBike->render(renderer); // Player on top
            gameTimer->render(renderer);
            break;
            
        case MENU:
            mainMenu->render();
            break;
            
        case GAME_OVER: {
            // Background rendering
            background->render();
            gameMap->render();
            background->renderRoad();
            
            // Overlay
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderFillRect(renderer, &overlay);
            
            TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 48);
            if (font) {
                SDL_Color color = {255, 0, 0, 255};
                const char* message = "GAME OVER";
                
                if (gameMap->isFinished(playerBike->getX())) {
                    message = "VICTORY!";
                    color = {0, 255, 0, 255};
                } else if (gameTimer->isTimeOver()) {
                    message = "TIME'S UP!";
                }
                
                SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        SDL_Rect rect = {
                            (SCREEN_WIDTH - surface->w)/2,
                            (SCREEN_HEIGHT - surface->h)/2,
                            surface->w,
                            surface->h
                        };
                        SDL_RenderCopy(renderer, texture, NULL, &rect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
                
                SDL_Color white = {255, 255, 255, 255};
                SDL_Surface* instr = TTF_RenderText_Solid(font, "Press R to restart or ESC for menu", white);
                if (instr) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, instr);
                    if (tex) {
                        SDL_Rect rect = {
                            (SCREEN_WIDTH - instr->w)/2,
                            (SCREEN_HEIGHT - instr->h)/2 + 80,
                            instr->w,
                            instr->h
                        };
                        SDL_RenderCopy(renderer, tex, NULL, &rect);
                        SDL_DestroyTexture(tex);
                    }
                    SDL_FreeSurface(instr);
                }
                
                TTF_CloseFont(font);
            }
            break;
        }
            
        case ABOUT: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
            SDL_RenderClear(renderer);
            
            TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 24);
            if (font) {
                SDL_Color white = {255, 255, 255, 255};
                
                SDL_Surface* title = TTF_RenderText_Solid(font, "BIKE RACE GAME", white);
                if (title) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, title);
                    if (tex) {
                        SDL_Rect rect = {(SCREEN_WIDTH - title->w)/2, 50, title->w, title->h};
                        SDL_RenderCopy(renderer, tex, NULL, &rect);
                        SDL_DestroyTexture(tex);
                    }
                    SDL_FreeSurface(title);
                }
                
                const char* lines[] = {
                    "Controls:",
                    "- LEFT/RIGHT arrows to move // problem her this in future will solve it ",
                    "- UP arrow or SPACE to jump",
                    "- R key to restart the game",
                    "- ESC to pause/return to menu",
                    "",
                    "Objective:",
                    "Avoid obstacles for 2 minutes",
                    "and reach the finish line!",
                    "",
                    "Press ESC to return to menu"
                };
                
                for (int i = 0; i < 11; i++) {
                    SDL_Surface* surface = TTF_RenderText_Solid(font, lines[i], white);
                    if (surface) {
                        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
                        if (tex) {
                            SDL_Rect rect = {
                                (SCREEN_WIDTH - surface->w)/2,
                                120 + i * 30,
                                surface->w,
                                surface->h
                            };
                            SDL_RenderCopy(renderer, tex, NULL, &rect);
                            SDL_DestroyTexture(tex);
                        }
                        SDL_FreeSurface(surface);
                    }
                }
                
                TTF_CloseFont(font);
            }
            break;
        }
    }
    
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete background;
    delete playerBike;
    delete gameMap;
    delete mainMenu;
    delete gameTimer;
    delete gameResources;
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    lastFrameTime = SDL_GetTicks();
    
    while (isRunning) {
        handleEvents();
        update();
        render();
        
        // Cap frame rate
        Uint32 currentTime = SDL_GetTicks();
        Uint32 frameTime = currentTime - lastFrameTime;
        
        if (frameTime < 16) {  // Target ~60 FPS
            SDL_Delay(16 - frameTime);
        }
    }
}