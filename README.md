- 👋 Hi, I’m @amrspace1
-# 🚴‍♂️ Bike Race - C++ SDL2 Game

**Bike Race** is a simple and fun 2D racing game built in C++ using the SDL2 library. It features a countdown timer, basic collision mechanics, keyboard controls, and a responsive graphical interface.

## 🕹️ How to Play

- Navigate the bike using arrow keys (or other keys defined in your code).
- Finish the course **before the timer runs out**.
- Avoid hitting obstacles or walls — collisions will end the game.
- Choose from menu options:  
  - `Jouer` to play  
  -`restart` to rastart the game
  - `À propos` to view information about the game  
  - `Quitter` to exit the application

---

## 💻 Development Environment

This game was developed with:

- C++17
- SDL2, SDL2_image, SDL2_ttf
- Visual Studio Code
- MSYS2 for compiling on Windows

---

## 🛠️ Build Instructions (VS Code + MSYS2)

### 1. Install MSYS2

Download and install MSYS2 from [https://www.msys2.org](https://www.msys2.org)

### 2. Open MSYS2 and install required packages:

```bash
pacman -Syu       # First-time full system update (you may need to close and reopen MSYS2 after this)
pacman -S mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_ttf
```

### 3. Open  vs code and copy this project and use this commen to run it:
``` terminal
 g++ -Isrc/Include -Lsrc/lib -o BikeRacingGame main.cpp Game.cpp Bike.cpp Map.cpp Obstacle.cpp Timer.cpp Menu.cpp CollisionDetection.cpp Resources.cpp Background.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
```
<!---
amrspace1/amrspace1 is a ✨ special ✨ repository because its `README.md` (this file) appears on your GitHub profile.
You can click the Preview link to take a look at your changes.
--->

