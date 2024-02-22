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
        float speed = 2.0f;

        float targetTiltAngle = 0.0f;
        if (IsKeyDown(KEY_RIGHT))
        {
            player.x += speed;
            targetTiltAngle = -15.0f; // Tilt right
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            player.x -= speed;
            targetTiltAngle = 15.0f; // Tilt left
        }

        if (IsKeyDown(KEY_DOWN))
            player.y += speed;
        else if (IsKeyDown(KEY_UP))
        {
            player.y -= speed;

            // Tilt from the top only when both top and left or top and right keys are pressed
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
                targetTiltAngle = -15.0f;
        }

        // Check if the spacecraft reached the screen edge, and reset position to the origin
        if (player.x > screenWidth || player.x < 0 || player.y > screenHeight || player.y < 0)
        {
            // Reset spacecraft position to the center (origin)
            player.x = screenWidth / 2 - 20;
            player.y = screenHeight / 2 - 20;
        }

        // Smoothly interpolate tilt angle
        float tiltInterpolation = 0.1f;
        camera.rotation += (targetTiltAngle - camera.rotation) * tiltInterpolation;

        // Camera target follows player
        camera.target = (Vector2){player.x + 20, player.y + 20};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE); // Clear the background to white

        BeginMode2D(camera);

        // Draw the space background without tilt
        DrawTexture(spaceBackground, -screenWidth / 2 - camera.target.x, -screenHeight / 2 - camera.target.y, WHITE);

        // Draw the spacecraft image with smooth tilt effect
        DrawTextureEx(spacecraftTexture, (Vector2){player.x, player.y}, camera.rotation, 1.0f, WHITE);

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
