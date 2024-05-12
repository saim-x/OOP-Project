#include "include/raylib.h"
#include <cmath>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

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
    Texture2D heartTexture;
};

HealthBar CreateHealthBar(float x, float y, float width, float height, Color outerColor, Color innerColor, int startingHealth)
{
    HealthBar bar;
    bar.outerRect = {x, y, width, height};
    bar.innerRect = {x + 2, y + 2, width - 4, height - 4};
    bar.outerColor = outerColor;
    bar.innerColor = innerColor;
    bar.currentHealth = startingHealth;
    bar.heartTexture = LoadTexture("media/heart.png");
    return bar;
}

void DrawHealthBar(HealthBar bar)
{
    // SIMPLE DRAWING OF HEALTH BAR
    //  // Draw outer rectangle
    //  DrawRectangleRec(bar.outerRect, bar.outerColor);

    // // Calculate inner rectangle width based on current health
    // float innerWidth = (bar.currentHealth / (float)maxHealth) * bar.innerRect.width;
    // bar.innerRect.width = (innerWidth < 0) ? 0 : innerWidth;

    // // Draw inner rectangle
    // DrawRectangleRec(bar.innerRect, bar.innerColor);

    DrawRectangleRounded(bar.outerRect, 0.5, 1, bar.outerColor);

    // Calculate inner rectangle width based on current health
    float innerWidth = (bar.currentHealth / (float)maxHealth) * bar.innerRect.width;
    bar.innerRect.width = (innerWidth < 0) ? 0 : innerWidth;

    // Draw inner rectangle with rounded corners
    DrawRectangleRounded(bar.innerRect, 0.2, 1, bar.innerColor);

    // Draw heart texture at the center of the health bar
    Vector2 heartPos = {bar.outerRect.x + (bar.outerRect.width - bar.heartTexture.width) / 2,
                        bar.outerRect.y + (bar.outerRect.height - bar.heartTexture.height) / 2};
    DrawTexture(bar.heartTexture, (int)heartPos.x, (int)heartPos.y, WHITE);
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

void SaveToFile(float score)
{
    ofstream outputFile("scores.txt", ios::app); // Open the file in append mode

    if (outputFile.is_open())
    {
        outputFile << score << endl; // Write the score to the file on a new line
        outputFile.close();          // Close the file
    }
    else
    {
        cout << "Failed to open the file for writing." << endl;
    }
}
void ShowAboutDevInfo()
{
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "About Developers");

    // Load the background image
    Texture2D AboutBackground = LoadTexture("saim's related vault/aboutthedev.png");

    // Play background music
    InitAudioDevice();
    Sound bgmusicforaboutscreen = LoadSound("saim's related vault/motivationalmusic.wav");
    PlaySound(bgmusicforaboutscreen);
    SetSoundVolume(bgmusicforaboutscreen, 1.0f);

    while (!WindowShouldClose())
    {
        // Close window by pressing ESC key
        if (IsKeyDown(KEY_ESCAPE))
        {
            StopSound(bgmusicforaboutscreen);
            break;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the background image
        DrawTexture(AboutBackground, 0, 0, WHITE);

        EndDrawing();
    }

    // Unload the background image
    UnloadTexture(AboutBackground);

    // Close the window after the loop
    CloseWindow();
}

void ShowHighScore()
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "HighScores Window");
    Camera2D camera = {0};
    camera.offset = Vector2({screenWidth / 2.0f, screenHeight / 2.0f});
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    SetTargetFPS(60);                 // Set our game to run at 60 frames-per-second
    ifstream inputFile("scores.txt"); // Open the file for reading
    vector<int> topScores;            // Vector to store the top scores
    string highScores;                // String to store all the high scores
    int topScore = 0;                 // Variable to store the top score
    InitAudioDevice();
    Sound bgmusicforhighscorescreen = LoadSound("resources/highscore.wav");
    PlaySound(bgmusicforhighscorescreen);
    SetSoundVolume(bgmusicforhighscorescreen, 2.6f);
    if (inputFile.is_open())
    {

        string score;
        while (getline(inputFile, score)) // Read each line from the file
        {
            // Convert the score to an integer
            int scoreValue = stoi(score);
            topScores.push_back(scoreValue);
        }
        inputFile.close(); // Close the file

        // Sort the scores in descending order
        sort(topScores.begin(), topScores.end(), greater<int>());

        // Resize the vector to contain only the top 5 scores
        int NumberOfScores = 5;

        if (topScores.size() > NumberOfScores)
        {
            topScores.resize(NumberOfScores);
        }
    }
    else
    {
        cout << "Failed to open the file for reading." << endl;
    }

    // Load the background image

    Texture2D spaceBackground = LoadTexture("saim's related vault/leaderboardtemp.png");

    while (!WindowShouldClose())
    {
        // Close window by pressing ESC key
        if (IsKeyDown(KEY_ESCAPE))
        {
            StopSound(bgmusicforhighscorescreen);
            break;
        }

        BeginDrawing();

        // Draw the background image
        DrawTexture(spaceBackground, 0, 0, WHITE);

        ClearBackground(RAYWHITE);

        // // Display the high scores on the screen
        // DrawText("High Scores", screenWidth / 2 - MeasureText("High Scores", 60) / 2, 50, 60, WHITE);
        // // Drawing the topscore
        // DrawText(TextFormat("Top Score: %d", topScore), screenWidth / 2 - MeasureText(TextFormat("Top Score: %d", topScore), 66) / 2, 120, 66, WHITE);
        // // Draw the high scores below the heading
        // DrawText(highScores.c_str(), screenWidth / 2 - MeasureText(highScores.c_str(), 44) / 2, 200, 44, RED);
        // EndDrawing();
        // Display the high scores on the screen
        // FILHAL K LIYE
        // DrawText("High Scores", screenWidth / 2 - MeasureText("High Scores", 60) / 2, 50, 60, WHITE);

        // Draw the top 5 scores with gaps
        for (size_t i = 0; i < topScores.size(); ++i)
        {
            DrawText(TextFormat("%1d. %1d\n\n\n", i + 1, topScores[i]), screenWidth / 2 - 180, 220 + i * 124, 36, WHITE);
        }
        EndDrawing();
    }

    CloseWindow(); // Close the window after the loop
    // Unload the background image
    UnloadTexture(spaceBackground);
}

// Function to run the game loop
void RunGame()
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "2D Space Game");
    HealthBar healthBar = CreateHealthBar(50, 50, 200, 30, WHITE, RED, maxHealth);
    InitAudioDevice();
    Rectangle player = {0, 0, 40, 40};
    Vector2 playerVelocity = {0.0f, 0.0f};
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    const float boundaryLeft = -670.0f;
    const float boundaryRight = 600.0f;
    const float boundaryTop = -300.0f;
    const float boundaryBottom = 250.0f;

    Camera2D camera = {0};
    camera.offset = Vector2({screenWidth / 2.0f, screenHeight / 2.0f});
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Load the initial space background image
    Texture2D spaceBackground;
    // Random Background DEPRECATED
    //  int ran = GetRandomValue(0, 1); // Generate a random number between 0 and 1 for variety background

    // if (ran == 0)
    // {
    //     spaceBackground = LoadTexture("media/space3.png");
    // }
    // else
    // {
    //     spaceBackground = LoadTexture("media/space2.png");
    // }
    // FIX BACKGROUND NOW
    spaceBackground = LoadTexture("media/space2.png");
    bool boostersActivated;

    // Load the spacecraft image
    Texture2D spacecraftTexture = LoadTexture("media/spacecraft23.png");

    // Load the background music
    Sound bgMusic = LoadSound("resources/bgmusicwav.wav"); // SUFYAN WALA MUSIC
    Sound sfx4 = LoadSound("resources/StopIt.wav");

    Sound sfx5 = LoadSound("resources/woosh.wav");
    Sound sfx6 = LoadSound("resources/randomsfx1.wav");
    Sound sfx7 = LoadSound("resources/randomsfx2.wav");

    Sound gameover = LoadSound("resources/GameOver.wav");
    // TESTING ONLY START HERE
    // TESTING ENDS HERE

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
    SetSoundVolume(bgMusic, 0.6f);

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

            if (IsKeyDown(KEY_F))
            {
                // Toggle BOOSTERS
                boostersActivated = true;
                PlaySound(sfx5);

                targetSpeedX *= 25.0f;
                targetSpeedY *= 25.0f;
                SetSoundVolume(sfx5, 3.9f);
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

            // Update player position based on velocityy
            player.x += playerVelocity.x;
            player.y += playerVelocity.y;

            // Spawn enemies randomly and limit the number of eenemies
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
                Rectangle enemyRect = {enemies[i].position.x, enemies[i].position.y + 20, static_cast<float>(enemies[i].texture.width) - 25, static_cast<float>(enemies[i].texture.height) - 10};
                if (CheckCollisionRecs(playerRect, enemyRect))
                {
                    PlaySound(gameover);

                    gameOver = true; // Game over if collision detected
                    SaveToFile(score);
                    break;
                }
            }
            // Fire bullets when 'F' key is pressed
            // if (IsKeyPressed(KEY_F) && !fKeyPressed)
            // {
            //     // Spawn bullet
            //     // Update bullet position
            //     // Set fKeyPressed to true
            //     fKeyPressed = true;
            // }
            // else if (!IsKeyPressed(KEY_F))
            // {
            //     // Reset fKeyPressed when the key is released
            //     fKeyPressed = false;
            // }

            // // Update and draw bullets
            // for (size_t i = 0; i < bullets.size(); i++)
            // {
            //     if (bullets[i].active)
            //     {
            //         // Update bullet position
            //         bullets[i].position.x += bullets[i].velocity.x;
            //         bullets[i].position.y += bullets[i].velocity.y;

            //         // Debug print statement
            //         printf("Bullet position: (%.2f, %.2f)\n", bullets[i].position.x, bullets[i].position.y);

            //         // Draw bullet
            //         DrawRectangle(bullets[i].position.x, bullets[i].position.y, 4, 4, RED);
            //     }
            // }
            // SFX FOR ENEMIES
            if (GetRandomValue(0, 300) < 1)
            {
                PlaySound(sfx4);
                SetSoundVolume(sfx4, 2.0f);
            }
            else if (GetRandomValue(0, 300) < 1)
            {
                PlaySound(sfx6);
                SetSoundVolume(sfx6, 1.0f);
            }
            else if (GetRandomValue(0, 300) < 1)
            {
                PlaySound(sfx7);
                SetSoundVolume(sfx7, 1.0f);
            }
        }
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        DrawTexture(spaceBackground, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
        DrawTextureEx(spacecraftTexture, Vector2({player.x, player.y}), 0.0f, 1.0f, WHITE);

        // Draw enemies
        for (const auto &enemy : enemies)
        {
            DrawTextureEx(enemy.texture, enemy.position, 0.0f, 1.0f, WHITE);
        }

        EndMode2D();

        DrawText("Space Shooter", 10, 10, 20, RED);
        if (gameOver)
        {

            StopSound(bgMusic);
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 20, 40, RED);
            DrawText(TextFormat("Your Score: %.2f", score), screenWidth / 2 - MeasureText("Your Score: xxxxxx", 20) / 2, (screenHeight / 2) + 55, 26, WHITE);
            DrawText(TextFormat("Time: %.2f seconds", gameTime), screenWidth / 2 - MeasureText(TextFormat("Time: %.2f seconds", gameTime), 20) / 2, (screenHeight / 2) + 80, 20, WHITE);
            DrawText(TextFormat("Score: %.2f ", score), screenWidth - MeasureText(TextFormat("%.2f seconds", score), 20) - 10, 40, 20, WHITE);
            DrawText("Press SPACE to Restart", screenWidth / 2 - MeasureText("Press SPACE to Restart xxx", 20) / 2, (screenHeight / 2) + 120, 26, WHITE);
        }
        else
        {

            DrawText(TextFormat("Score: %.2f ", score), screenWidth - MeasureText(TextFormat("%.2f seconds", score), 20) - 10, 10, 20, WHITE);
            // Draw legend
            DrawText("Arrows: Move", screenWidth - MeasureText("Arrows: Move", 20) - 10, screenHeight - 60, 20, WHITE);
            DrawText("F: Boost", screenWidth - MeasureText("F: Boost", 20) - 10, screenHeight - 30, 20, WHITE);
            DrawText("Exit: Escape", screenWidth - MeasureText("Exit: Escape", 20) - 10, screenHeight - 100, 20, WHITE);
        }

        // Update and draw health bar or enemy counter
        healthBar.currentHealth = enemies.size() * 20;
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
                healthBar.currentHealth = 0;
                boostersActivated = false;
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
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Space Shooter - Main Menu");

    // Load the background image
    Texture2D backgroundImage = LoadTexture("media\\bgimage1600main.png");

    // Adjust the background image rectangle to cover the entire window
    Rectangle bgRec = {0, 0, (float)screenWidth, (float)screenHeight};

    Rectangle playButton = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 100), 250, 50};
    Rectangle highScoreButton = {(float)(screenWidth / 2 + 10), (float)(screenHeight / 2 - 20), 140, 50};
    Rectangle aboutButton = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 + -20), 100, 50};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the background image
        DrawTexturePro(backgroundImage, Rectangle({0.0f, 0.0f, (float)backgroundImage.width, (float)backgroundImage.height}), bgRec, Vector2({0, 0}), 0.0f, WHITE);

        // Draw play button
        DrawRectangleRec(playButton, BLUE);
        DrawText("Play", (int)playButton.x + 100, (int)playButton.y + 15, 20, WHITE);

        // Draw high score button
        DrawRectangleRec(highScoreButton, GREEN);
        DrawText("High Score", (int)highScoreButton.x + 10, (int)highScoreButton.y + 15, 20, WHITE);

        // Draw about button
        DrawRectangleRec(aboutButton, RED);
        DrawText("About", (int)aboutButton.x + 25, (int)aboutButton.y + 15, 20, WHITE);
        //YE text ab print nhi krwana hai kyoke mene bg image me hi krdia hai
        // Draw game name
        // DrawText("SPACE SHOOTER GAME", screenWidth / 2 - MeasureText("SPACE SHOOTER GAME", 32) / 2, (screenHeight / 2) + 55, 32, WHITE);
        // DrawText("Developed By:\n\nSaim\n\nSufyan\n\nTalha", screenWidth / 2 - MeasureText("Developed By:\n\nSaim\n\nSufyan\n\nTalha", 26) / 2, (screenHeight / 2) + 100, 26, RED);

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
        if (CheckCollisionPointRec(GetMousePosition(), highScoreButton))
        {
            DrawRectangleLinesEx(highScoreButton, 3, BLACK); // Highlight the button if the mouse is over it

            // Check if the left mouse button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Display high score (for now, just print a message)
                CloseWindow();
                // Close the main screen window
                ShowHighScore(); // Show the high score screen
            }
        }
        if (CheckCollisionPointRec(GetMousePosition(), aboutButton))
        {
            DrawRectangleLinesEx(aboutButton, 3, BLACK); // Highlight the button if the mouse is over it

            // Check if the left mouse button is clicked
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Display high score (for now, just print a message)
                CloseWindow();
                // Close the main screen window
                ShowAboutDevInfo(); // Show the high score screen
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
