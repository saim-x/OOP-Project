#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>

// Define the Enemy structure
struct Enemy
{
    Vector2 position;
    Texture2D texture;
    float speed;
};

// Function to initialize an enemy character at a random position within the boundaries
Enemy InitEnemy(const Rectangle &boundary)
{
    Enemy enemy;
    enemy.position.x = GetRandomValue(boundary.x, boundary.x + boundary.width);
    enemy.position.y = GetRandomValue(boundary.y, boundary.y + boundary.height);
    // Randomly choose between enemy1 and enemy2 textures
    if (GetRandomValue(0, 1) == 0)
    {
        enemy.texture = LoadTexture("media/enemy1.png");
    }
    else
    {
        enemy.texture = LoadTexture("media/enemy2.png");
    }
    enemy.speed = 2.0f; // Set enemy speed
    return enemy;
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "2D Space Game");

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

    // Seed the random number generator
    srand(time(NULL));

    // Initialize vector to store enemies
    std::vector<Enemy> enemies;

    bool gameOver = false;
    bool restartRequested = false; // Flag to track if restart has been requested
    float gameTime = 0.0f;         // Variable to track elapsed game time

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (!gameOver) // Only update the game if it's not over
        {
            gameTime += GetFrameTime(); // Update game time

            // Update player movement
            float targetSpeedX = 0.0f;
            float targetSpeedY = 0.0f;

            if (IsKeyDown(KEY_RIGHT) && player.x < boundaryRight)
            {
                targetSpeedX += acceleration;
            }
            else if (IsKeyDown(KEY_LEFT) && player.x > boundaryLeft)
            {
                targetSpeedX -= acceleration;
            }

            if (IsKeyDown(KEY_DOWN) && player.y < boundaryBottom)
            {
                targetSpeedY += acceleration;
            }
            else if (IsKeyDown(KEY_UP) && player.y > boundaryTop)
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

            // Spawn enemies randomly and limit the number of enemies
            if (GetRandomValue(0, 100) < 1 && enemies.size() < 5) // Adjust spawn rate and max enemies as needed
            {
                enemies.push_back(InitEnemy({boundaryLeft, boundaryTop, boundaryRight - boundaryLeft, boundaryBottom - boundaryTop}));
            }

            // Update enemy positions
            for (size_t i = 0; i < enemies.size(); i++)
            {
                // Move enemies towards the player (You can update this logic as per your requirement)
                Vector2 direction = {player.x - enemies[i].position.x, player.y - enemies[i].position.y};
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                direction.x /= distance;
                direction.y /= distance;
                enemies[i].position.x += direction.x * enemies[i].speed;
                enemies[i].position.y += direction.y * enemies[i].speed;

                // Check for collision with player
                Rectangle playerRect = {player.x, player.y, player.width, player.height};
                Rectangle enemyRect = {enemies[i].position.x, enemies[i].position.y, static_cast<float>(enemies[i].texture.width), static_cast<float>(enemies[i].texture.height)};
                if (CheckCollisionRecs(playerRect, enemyRect))
                {
                    gameOver = true; // Game over if collision detected
                    break;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        DrawTexture(spaceBackground, -screenWidth / 2 - camera.target.x, -screenHeight / 2 - camera.target.y, WHITE);
        DrawTextureEx(spacecraftTexture, (Vector2){player.x, player.y}, 0.0f, 1.0f, WHITE);

        // Draw enemies
        for (const auto &enemy : enemies)
        {
            DrawTextureEx(enemy.texture, enemy.position, 0.0f, 1.0f, WHITE);
        }

        EndMode2D();

        DrawText("Space Shooter", 10, 10, 20, RED);
        if (gameOver)
        {
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 20, 40, RED);
            DrawText("Your Score: ", screenWidth / 2 - MeasureText("Your Score: ", 20) / 2, (screenHeight / 2)+40, 20, WHITE);
            DrawText(TextFormat("%.2f seconds", gameTime), screenWidth / 2 - MeasureText(TextFormat("%.2f seconds", gameTime), 20) / 2, (screenHeight / 2) + 80, 20, WHITE);

            DrawText("Press SPACE to Restart", screenWidth / 2 - MeasureText("Press SPACE to Restart", 20) / 2, (screenHeight / 2 )+ 120, 26, WHITE);
        }
        else
        {
            DrawText("Developed By Saim", screenWidth - 150, screenHeight - 30, 10, YELLOW);
        }

        EndDrawing();

        if (gameOver)
        {
            if (IsKeyDown(KEY_SPACE))
            {
                // Reset game variables for restart
                player = {0, 0, 40, 40};
                playerVelocity = {0.0f, 0.0f};
                enemies.clear();
                gameOver = false;
                restartRequested = true;
                gameTime = 0.0f; // Reset game time
            }
        }

        if (restartRequested && !IsKeyDown(KEY_SPACE))
        {
            restartRequested = false;
        }

        if (gameOver && IsKeyDown(KEY_ESCAPE))
        {
            break; // Exit game loop if game over and ESC key pressed
        }
    }

    // Unload textures
    UnloadTexture(spaceBackground);
    UnloadTexture(spacecraftTexture);

    CloseWindow();

    return 0;
}
