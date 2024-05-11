#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "2D Space Game");
    InitAudioDevice();
    Rectangle player = {0, 0, 40, 40};
    Vector2 playerVelocity = {0.0f, 0.0f};
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    const float boundaryLeft = -440.0f;
    const float boundaryRight = 310.0f;
    const float boundaryTop = -410.0f;
    const float boundaryBottom = 350.0f;

    Camera2D camera = {0};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Load the initial space background image
    Texture2D spaceBackground = LoadTexture("media/space.png");

    // Load the spacecraft image
    Texture2D spacecraftTexture = LoadTexture("media/spacecraft23.png");

    // Load the background music
    Sound bgMusic = LoadSound("resources/random.wav");

    // Seed the random number generator
    srand(time(NULL));
    std::vector<Enemy> enemies;

    // Initialize vector to store bullets
    std::vector<Bullet> bullets;

    bool gameOver = false;
    bool restartRequested = false; // Flag to track if restart has been requested
    float gameTime = 0.0f;         // Variable to track elapsed game time

    // Play background music
    PlaySound(bgMusic);