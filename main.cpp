#include "include/raylib.h"
#include <cmath>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "2D Space Game");

    Rectangle player = {screenWidth / 2 - 20, screenHeight / 2 - 20, 40, 40};
    Vector2 playerVelocity = {0.0f, 0.0f};
    const float maxSpeed = 26.0f; // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    Camera2D camera = {0};
    camera.target = (Vector2){player.x + 20.0f, player.y + 20.0f};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Load the initial space background image
    Texture2D spaceBackground = LoadTexture("media/space.png");

    // Load the spacecraft image
    Texture2D spacecraftTexture = LoadTexture("media/spacecraft.png");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Player movement
        float targetSpeedX = 0.0f;
        float targetSpeedY = 0.0f;

        if (IsKeyDown(KEY_RIGHT))
        {
            targetSpeedX += acceleration;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            targetSpeedX -= acceleration;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            targetSpeedY += acceleration;
        }
        else if (IsKeyDown(KEY_UP))
        {
            targetSpeedY -= acceleration;
        }

        // Smoothly accelerate/decelerate towards target speed
        if (targetSpeedX > playerVelocity.x)
        {
            playerVelocity.x = fminf(playerVelocity.x + acceleration, maxSpeed);
        }
        else if (targetSpeedX < playerVelocity.x)
        {
            playerVelocity.x = fmaxf(playerVelocity.x - acceleration, -maxSpeed);
        }
        else
        {
            playerVelocity.x *= deceleration;
        }

        if (targetSpeedY > playerVelocity.y)
        {
            playerVelocity.y = fminf(playerVelocity.y + acceleration, maxSpeed);
        }
        else if (targetSpeedY < playerVelocity.y)
        {
            playerVelocity.y = fmaxf(playerVelocity.y - acceleration, -maxSpeed);
        }
        else
        {
            playerVelocity.y *= deceleration;
        }

        // Update player position based on velocity
        player.x += playerVelocity.x;
        player.y += playerVelocity.y;

        // Update camera target based on player movement
        camera.target.x += playerVelocity.x;
        camera.target.y += playerVelocity.y;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE); // Clear the background to white

        BeginMode2D(camera);

        // Draw the space background
        // We draw it multiple times to cover the whole screen seamlessly
        for (int y = -screenHeight; y < screenHeight * 2; y += spaceBackground.height)
        {
            for (int x = -screenWidth; x < screenWidth * 2; x += spaceBackground.width)
            {
                DrawTexture(spaceBackground, x, y, WHITE);
            }
        }

        // Draw the spacecraft image at the player's position
        DrawTextureEx(spacecraftTexture, (Vector2){player.x, player.y}, 0.0f, 1.0f, WHITE);

        EndMode2D();

        DrawText("2D Space Game", 10, 10, 20, RED);

        DrawText("Developed By Saim", screenWidth - 150, screenHeight - 30, 10, YELLOW);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Unload the space background image and spacecraft image
    UnloadTexture(spaceBackground);
    UnloadTexture(spacecraftTexture);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
