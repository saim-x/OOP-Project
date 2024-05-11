#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;
// Define the Enemy
struct Enemy
{
    Vector2 position;
    Texture2D texture;
    float speed;
};

struct Bullet
{
    Vector2 position;
    Vector2 velocity;
    bool active;
};
int flag = 0;        // flag to check if boss enemy is spawned or not
int maxHealth = 100; // Maximum health of the player
struct HealthBar
{
    Rectangle outerRect;
    Rectangle innerRect;
    Color outerColor;
    Color innerColor;
    int currentHealth;
};

HealthBar CreateHealthBar(float x, float y, float width, float height, Color outerColor, Color innerColor, int startingHealth)
{
    HealthBar bar;
    bar.outerRect = {x, y, width, height};
    bar.innerRect = {x + 2, y + 2, width - 4, height - 4};
    bar.outerColor = outerColor;
    bar.innerColor = innerColor;
    bar.currentHealth = startingHealth;
    return bar;
}

void DrawHealthBar(HealthBar bar)
{
    // Draw outer rectangle
    DrawRectangleRec(bar.outerRect, bar.outerColor);

    // Calculate inner rectangle width based on current health
    float innerWidth = (bar.currentHealth / (float)maxHealth) * bar.innerRect.width;
    bar.innerRect.width = (innerWidth < 0) ? 0 : innerWidth;

    // Draw inner rectangle
    DrawRectangleRec(bar.innerRect, bar.innerColor);
}

// Function to initialize an enemy character at a random position within the boundaries
Enemy InitEnemy(const Rectangle &boundary, const Rectangle &player)
{
    Enemy enemy;
    enemy.position.x = GetRandomValue(boundary.x, boundary.x + boundary.width);
    enemy.position.y = GetRandomValue(boundary.y, boundary.y + boundary.height);
    // Logic to prevent enemies from overlapping the player by moving them apart by 50 units
    if (abs(enemy.position.x - player.x) <= 50 && abs(enemy.position.y - player.y) <= 50)
    {
        // Calculate the new enemy position 50 units away from the player
        float newX = enemy.position.x;
        float newY = enemy.position.y;

        if (enemy.position.x < player.x)
            newX -= 50;
        else
            newX += 50;

        if (enemy.position.y < player.y)
            newY -= 50;
        else
            newY += 50;

        // Check if the new position is within the window boundaries
        if (newX < boundary.x)
            newX = boundary.x;
        else if (newX > boundary.x + boundary.width)
            newX = boundary.x + boundary.width;

        if (newY < boundary.y)
            newY = boundary.y;
        else if (newY > boundary.y + boundary.height)
            newY = boundary.y + boundary.height;

        // Update the enemy position
        enemy.position.x = newX;
        enemy.position.y = newY;
    }
    enemy.speed = GetRandomValue(15, 30) / 10.0f; // Set enemy speed randomly from 1.5 to 3.0

    // Load the boss enemy sfx
    Sound sfx1 = LoadSound("resources/sfx1edited.wav");
    Sound sfx2 = LoadSound("resources/poinkwav.wav");
    // BOSS HAVE TO SPAWN ONCE LOGIC
    if (flag == 0)
    {
        flag = 1;
        // BOSS ENEMY WILL SPAWN ONLY ONCE :D
        enemy.texture = LoadTexture("media/enemy3.1.png");
        PlaySound(sfx1);
        enemy.speed = 3.0f; // Set boss enemy speed to 3.0 which is max an enemy can have
    }
    // Randomly choose between enemy1 and enemy2 textures
    else if (GetRandomValue(0, 1) == 0)
    {
        enemy.texture = LoadTexture("media/enemy1.png");
        PlaySound(sfx2);
    }
    else
    {
        enemy.texture = LoadTexture("media/enemy3.png");
        PlaySound(sfx2);
    }
    return enemy;
}

Bullet InitBullet(Vector2 position, Vector2 velocity)
{
    Bullet bullet;
    bullet.position = position;
    bullet.velocity = velocity;
    bullet.active = true;
    return bullet;
}
float score = 0.0f;
// Function to run the game loop
void RunGame()
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "2D Space Game");
    HealthBar healthBar = CreateHealthBar(50, 50, 200, 30, GREEN, RED, maxHealth);
    InitAudioDevice();
    Rectangle player = {0, 0, 40, 40};
    Vector2 playerVelocity = {0.0f, 0.0f};
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    const float boundaryLeft = -815.0f;
    const float boundaryRight = 715.0f;
    const float boundaryTop = -429.0f;
    const float boundaryBottom = 332.0f;

    Camera2D camera = {0};
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Load the initial space background image
    Texture2D spaceBackground;
    int ran = GetRandomValue(0, 1); // Generate a random number between 0 and 1 for variety background

    if (ran == 0) {
        spaceBackground = LoadTexture("media/space3.png");
    } else {
        spaceBackground = LoadTexture("media/space2.png");
    }

    // Load the spacecraft image
    Texture2D spacecraftTexture = LoadTexture("media/spacecraft23.png");

    // Load the background music
    Sound bgMusic = LoadSound("resources/bgmusicwav.wav"); // SUFYAN WALA MUSIC

    // Seed the random number generator
    srand(time(NULL));
    vector<Enemy> enemies;

    // Initialize vector to store bullets
    vector<Bullet> bullets;

    bool gameOver = false;
    bool restartRequested = false; // Flag to track if restart has been requested
    float gameTime = 0.0f;         // Variable to track elapsed game time

    // Play background music
    PlaySound(bgMusic);

    SetTargetFPS(60);         // Set our game to run at 60 frames-per-second
    bool fKeyPressed = false; // Initialize outside your update loop

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (!gameOver) // Only update the game if it's not over
        {
            gameTime += GetFrameTime(); // Update game time
            score = score + (2 * GetFrameTime());

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
                enemies.push_back(InitEnemy({boundaryLeft, boundaryTop, boundaryRight - boundaryLeft, boundaryBottom - boundaryTop}, player));
            }

            // Update enemy positions
            for (size_t i = 0; i < enemies.size(); i++)
            {
                // Move enemies towards the player (You can update this logic as per your requirement)
                // float randomFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
                Vector2 direction = {player.x - enemies[i].position.x, player.y - enemies[i].position.y};
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                direction.x /= distance;
                direction.y /= distance;
                // READ THISSSSSS: Logic to prevent enemies from overlapping by moving them apart by 30 units
                // if (i < enemies.size() - 1)
                // {
                //     Rectangle enemyRectbysaimedition = {enemies[i].position.x - 10, enemies[i].position.y + 20, static_cast<float>(enemies[i].texture.width), static_cast<float>(enemies[i].texture.height)};
                //     Rectangle enemyRectbysaimeditionsecond = {enemies[i + 1].position.x - 10, enemies[i + 1].position.y + 20, static_cast<float>(enemies[i + 1].texture.width), static_cast<float>(enemies[i + 1].texture.height)};
                //     if (CheckCollisionRecs(enemyRectbysaimedition, enemyRectbysaimeditionsecond))
                //     {
                //         enemies[i].position.x += 30;
                //         enemies[i].position.y += 30;
                //     }
                // }

                enemies[i].position.x += (direction.x) * enemies[i].speed;
                enemies[i].position.y += (direction.y) * enemies[i].speed;

                // Check for collision with player
                Rectangle playerRect = {player.x + 40, player.y + 30, player.width - 35, player.height + 30};
                Rectangle enemyRect = {enemies[i].position.x , enemies[i].position.y + 20, static_cast<float>(enemies[i].texture.width)-25, static_cast<float>(enemies[i].texture.height)-10};
                if (CheckCollisionRecs(playerRect, enemyRect))
                {
                    gameOver = true; // Game over if collision detected
                    break;
                }
            }

            // Fire bullets when 'F' key is pressed
            if (IsKeyPressed(KEY_F) && !fKeyPressed)
            {
                // Spawn bullet
                // Update bullet position
                // Set fKeyPressed to true
                fKeyPressed = true;
            }
            else if (!IsKeyPressed(KEY_F))
            {
                // Reset fKeyPressed when the key is released
                fKeyPressed = false;
            }

            // Update and draw bullets
            for (size_t i = 0; i < bullets.size(); i++)
            {
                if (bullets[i].active)
                {
                    // Update bullet position
                    bullets[i].position.x += bullets[i].velocity.x;
                    bullets[i].position.y += bullets[i].velocity.y;

                    // Debug print statement
                    printf("Bullet position: (%.2f, %.2f)\n", bullets[i].position.x, bullets[i].position.y);

                    // Draw bullet
                    DrawRectangle(bullets[i].position.x, bullets[i].position.y, 4, 4, RED);
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        DrawTexture(spaceBackground, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
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
            DrawText(TextFormat("Your Score: %.2f", score), screenWidth / 2 - MeasureText("Your Score: xxxxxx", 20) / 2, (screenHeight / 2) + 55, 26, WHITE);
            DrawText(TextFormat("Time: %.2f seconds", gameTime), screenWidth / 2 - MeasureText(TextFormat("Time: %.2f seconds", gameTime), 20) / 2, (screenHeight / 2) + 80, 20, WHITE);
            DrawText(TextFormat("Score: %.2f ", score), screenWidth - MeasureText(TextFormat("%.2f seconds", score), 20) - 10, 40, 20, WHITE);
            DrawText("Press SPACE to Restart", screenWidth / 2 - MeasureText("Press SPACE to Restart xxx", 20) / 2, (screenHeight / 2) + 120, 26, WHITE);
        }
        else
        {
            DrawText(TextFormat("Score: %.2f ", score), screenWidth - MeasureText(TextFormat("%.2f seconds", score), 20) - 10, 10, 20, WHITE);
            DrawText("Developed By Saim", screenWidth - 150, screenHeight - 30, 10, YELLOW);
        }
        // Update and draw health bar
        healthBar.currentHealth -= 1; // Simulate health decrease
        DrawHealthBar(healthBar);
        EndDrawing();

        if (gameOver)
        {
            if (IsKeyDown(KEY_SPACE))
            {
                // Reset game variables for restart

                player = {0, 0, 40, 40};
                flag = 0;
                playerVelocity = {0.0f, 0.0f};
                enemies.clear();
                gameOver = false;
                restartRequested = true;
                gameTime = 0.0f;    // Reset game time
                PlaySound(bgMusic); // Play background music again
                score = 0.0f;
                Sound sfx3 = LoadSound("resources/gamerestart.mp3");
                PlaySound(sfx3);
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

    // Unload sound and textures
    UnloadSound(bgMusic);
    UnloadTexture(spaceBackground);
    UnloadTexture(spacecraftTexture);

    CloseWindow();
}

// Function to display the main menu screen 
void ShowMainMenu()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Space Shooter - Main Menu");

    // Load the background image
    Texture2D backgroundImage = LoadTexture("media\\bgimagepngggg.png");

    // Adjust the background image rectangle to cover the entire window
    Rectangle bgRec = {0, 0, (float)screenWidth, (float)screenHeight};

    Rectangle playButton = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 25), 100, 50};
    Rectangle highScoreButton = {(float)(screenWidth / 2 + 10), (float)(screenHeight / 2 - 25), 140, 50};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the background image
        DrawTexturePro(backgroundImage, (Rectangle){0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height}, bgRec, (Vector2){0, 0}, 0.0f, WHITE);

        // Draw play button
        DrawRectangleRec(playButton, BLUE);
        DrawText("Play", (int)playButton.x + 30, (int)playButton.y + 15, 20, WHITE);

        // Draw high score button
        DrawRectangleRec(highScoreButton, GREEN);
        DrawText("High Score", (int)highScoreButton.x + 10, (int)highScoreButton.y + 15, 20, WHITE);

        // Check if the mouse is hovering over the play button
        if (CheckCollisionPointRec(GetMousePosition(), playButton))
        {
            DrawRectangleLinesEx(playButton, 3, BLACK); // Highlight the button if the mouse is over it

            // Check if the left mouse button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Start the game
                CloseWindow(); // Close the main screen window
                RunGame();     // Start the game loop
            }
        }
        // Check if the mouse is hovering over the high score button
        if (CheckCollisionPointRec(GetMousePosition(), highScoreButton))
        {
            DrawRectangleLinesEx(highScoreButton, 3, BLACK); // Highlight the button if the mouse is over it

            // Check if the left mouse button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Display high score (for now, just print a message)
                cout << "High Score button clicked!" << endl;
            }
        }

        EndDrawing();
    }

    // Unload the background image
    UnloadTexture(backgroundImage);

    CloseWindow();
}

int main(void)
{
    ShowMainMenu();

    return 0;
}
