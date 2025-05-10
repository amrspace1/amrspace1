all:
  g++ -Isrc/Include -Lsrc/lib -o BikeRacingGame main.cpp Game.cpp Bike.cpp Map.cpp Obstacle.cpp 
  Timer.cpp Menu.cpp CollisionDetection.cpp Resources.cpp Background.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

run:
    ./BikeRacingGame
/*BikeGame/ structeure off the project
├── assets/
│   ├── images/
│   │   ├── bike.png
│   │   ├── wall.png
│   │   ├── ramp.png
│   │   ├── pit.png
│   │   ├── menu_bg.png
│   │   ├── bg_layer0.png (distant mountains)
│   │   ├── bg_layer1.png (midground trees)
│   │   └── bg_layer2.png (foreground bushes)
│   └── fonts/
│       └── arial.ttf
├── include/
│   ├── Bike.h
│   ├── CollisionDetection.h
│   ├── Constants.h
│   ├── Exception.h
│   ├── Game.h
│   ├── Map.h
│   ├── Menu.h
│   ├── Obstacle.h
│   ├── Resources.h
│   ├── Timer.h
│   └── Background.h
└── src/
    ├── Bike.cpp
    ├── CollisionDetection.cpp
    ├── Game.cpp
    ├── Map.cpp
    ├── Menu.cpp
    ├── Obstacle.cpp
    ├── Resources.cpp
    ├── Timer.cpp
    └── Background.cpp
main.cpp*/