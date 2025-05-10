#pragma once

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int GAME_TIME_LIMIT = 150;
const float BIKE_SPEED = 250.0f;
const float BIKE_FORWARD_SPEED = 250.0f;
const float MIN_BIKE_SPEED = 150.0f;    // New constant: Minimum bike speed
const float MAX_BIKE_SPEED = 450.0f;    // New constant: Maximum bike speed
const float BIKE_ACCELERATION = 100.0f; // New constant: Bike acceleration
const float BIKE_JUMP_FORCE = 800.0f;
const float GRAVITY = 900.0f;
const int OBSTACLE_SPAWN_INTERVAL = 7000;
const int GROUND_LEVEL = 620;
const int ROAD_Y_POSITION = 620;
const int ROAD_HEIGHT = 100;
const int BIKE_JUMP_HEIGHT = 300;
const int ROAD_WIDTH = 100;
const int ROAD_LEFT_MARGIN = 440;
const int ROAD_RIGHT_MARGIN = ROAD_LEFT_MARGIN + ROAD_WIDTH;
const int BIKE_START_X = ROAD_LEFT_MARGIN + (ROAD_WIDTH / 2);
const int CENTER_SAFE_ZONE = 10;
const float EDGE_SPAWN_PROBABILITY = 0.7f;
const float CENTER_SPAWN_PROBABILITY = 0.2f;

enum ObstacleType {
    WALL,
    RAMP,
    PIT
};