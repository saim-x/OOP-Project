#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class Bullet;
// Global Variables
int flag = 0;        // flag to check if boss enemy is spawned or not
int maxHealth = 100; // Maximum health of the player

const int screenWidth = 1600;
const int screenHeight = 850;

const float boundaryLeft = -815.0f;
const float boundaryRight = 715.0f;
const float boundaryTop = -429.0f;
const float boundaryBottom = 332.0f;

class Game
{
protected:
    bool gameover;
    Camera2D camera = {0};
    char *background;
    Texture2D backgroundtexture;
    Rectangle player;
    char *texture;
    Texture2D textureobject;
    Vector2 playervelocity;
    float speed;
    char *music;
    Sound bgMusic;
    float gameTime_;

public:
    Game(char *texture, char *music, char *background) : texture(texture), music(music), background(background), gameTime_(0.0)
    { // for player
        speed = 3.0f;
        gameover = false;
        camera.offset = Vector2({screenWidth / 2.0f, screenHeight / 2.0f});
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        backgroundtexture = LoadTexture(background);
        playervelocity.x = 0.0f;
        playervelocity.y = 0.0f;
        player.x = 0;
        player.y = 0;
        player.width = 40;
        player.height = 40;
        textureobject = LoadTexture(texture);
        bgMusic = LoadSound(music);
        BeginMode2D(camera);
        DrawTexture(backgroundtexture, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
        Vector2 playingPosition_ = {player.x, player.y};
        DrawTextureEx(textureobject, playingPosition_, 0.0f, 1.0f, WHITE);
    }
    Game(float x, float y, char *texture) : texture(texture)
    { // for enemy
        textureobject = LoadTexture(texture);
        player.height = textureobject.height;
        player.width = textureobject.width;
    }
    ~Game()
    {
        UnloadTexture(textureobject);
        UnloadSound(bgMusic);
    }
    virtual void setpos(float x, float y) = 0;
    virtual void draw() {}
    float getx() { return player.x; }
    float gety() { return player.y; }
    float getwidth() { return player.height; }
    float getheight() { return player.width; }
    bool GetGameover() { return gameover; }
    Rectangle getrect() { return player; }
    float get_gameTime() const { return gameTime_; }
};

class Player : public Game
{
protected:
    float score;

    // For Bullets
    double lastFireTime_;
    Bullet bullet;

public:
    friend class Bullet;
    Player(char *texture, char *music, char *background) : Game(texture, music, background), bullet(Vector2{0, 0}, 0.6)
    {
        score = 0;
        lastFireTime_ = 0.0;
    }
    void setpos(float x, float y)
    {
        // We can try operator overloading here.
        gameTime_ += GetFrameTime();
        scoreinc(2 * GetFrameTime());
        player.x += x;
        player.y += y;
    }
    void Gameover()
    {
        gameover = true;
        player.x = 0;
        player.y = 0;
    }
    void draw()
    {
        BeginMode2D(camera);
        DrawTexture(backgroundtexture, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
        DrawTextureEx(textureobject, Vector2({player.x, player.y}), 0.0f, 1.0f, WHITE);
    }

    template <typename T>
    void scoreinc(T increase)
    {
        score += increase;
    }
    float getscore() { return score; }

    // Return Music from Game Class
    Sound get_bgMusic() const { return bgMusic; }
    // Return GameOver from Game Class
    bool get_gameover() const { return gameover; }
    // Return x-coordinate of Player
    float get_x() const { return player.x; }
    // Return y-coordinate of Player
    float get_y() const { return player.y; }
    // Return Width of Player
    float get_width() const { return player.width; }
    // Return Height of Player
    float get_height() const { return player.height; }

    Rectangle getbulletrect() { return bullet.getbullet(); }
};

class Enemy : public Game
{
protected:
    bool alive;

public:
    Enemy(float x, float y, char *texture, bool boss) : Game(x, y, texture)
    {
        alive = true;
        player.x = GetRandomValue(boundaryLeft, boundaryRight);
        player.y = GetRandomValue(boundaryTop, boundaryBottom);
        if (boss)
            speed = 3.0f;
        else
            speed = GetRandomValue(15, 30) / 10.0f; // Set enemy speed randomly from 1.5 to 3.0
        if (abs(player.x - x) <= 50 && abs(player.y - y) <= 50)
        {
            // Calculate the new enemy position 50 units away from the player
            float newX = player.x;
            float newY = player.y;

            if (player.x < x)
                newX -= 50;
            else
                newX += 50;

            if (player.y < y)
                newY -= 50;
            else
                newY += 50;

            // Check if the new position is within the window boundaries
            if (newX < boundaryRight)
                newX = boundaryRight;
            else if (newX > boundaryLeft)
                newX = boundaryLeft;

            if (newY < boundaryBottom)
                newY = boundaryBottom;
            else if (newY > boundaryTop)
                newY = boundaryTop;

            // Update the enemy position
            player.x = newX;
            player.y = newY;
        }
    }
    void setpos(float x, float y)
    {
        player.x += x * speed;
        player.y += y * speed;
    }
    void draw()
    {
        if (alive)
        {
            DrawTextureEx(textureobject, Vector2({player.x, player.y}), 0.0f, 1.0f, WHITE);
        }
        else
        {
            player.x = 2000.0f;
            player.y = 2000.0f;
        }
    }
    bool getstatus() { return alive; }
    void setstatus() { alive = false; }

    // Return x-coordinate of Enemy
    float get_x() const { return player.x; }
    // Return y-coordinate of Enemy
    float get_y() const { return player.y; }
    //  Return Speed of Enemy
    float get_speed() const { return speed; }
    // Return Texture width of Enemy
    float get_textureWidth() const { return textureobject.width; }
    // Return Texture height of Enemy
    float get_textureHeight() const { return textureobject.height; }
};

class Bullet
{
private:
    Rectangle bullet;
    // Attributes
    Vector2 position_;
    float speed_;
    Texture2D bullettexture;
    bool moving;
    char key;

public:
    // Attribute
    bool active_; // Variable to check if bullet is still within the game window.

    //  Constructors
    Bullet() {}
    Bullet(const Vector2 position, const float speed) : position_(position), speed_(speed), active_(true)
    {
        bullettexture = LoadTexture("media/bulletbySufyan2");
        moving = false;
    }

    // Methods
    // Function to update the bullet's position.
    void Update(Player pobj)
    {
        if (IsKeyPressed(KEY_W))
        {
            moving = true;
            key = 'W';
        }
        else if (IsKeyPressed(KEY_A))
        {
            moving = true;
            key = 'A';
        }
        else if (IsKeyPressed(KEY_D))
        {
            moving = true;
            key = 'D';
        }
        else if (IsKeyPressed(KEY_S))
        {
            moving = true;
            key = 'S';
        }
        if (active_)
        {
            if (moving)
            {
                if (key == 'W')
                {
                    position_.y -= speed_;
                }
                else if (key == 'A')
                {
                    position_.x -= speed_;
                }
                else if (key == 'D')
                {
                    position_.x += speed_;
                }
                else if (key == 'S')
                {
                    position_.y += speed_;
                }
            }
            if (position_.y > 332.0f || position_.y < -429.0f || position_.x < -815.0f || position_.x > 715.0f)
            {
                active_ = false;
                moving = false;
            }
            DrawTextureEx(bullettexture, position_, 0.0f, 1.0f, WHITE);
        }
        else
        {
            position_.x = pobj.player.x;
            position_.y = pobj.player.y;
            moving = false;
        }
    }
    // Function to draw the bullet.
    Rectangle getbullet() { return bullet; }
};

class dValues
{
public:
    dValues() {}
    // Attributes
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    Sound bgMusic = LoadSound("resources/bgmusicwav.wav"); // SUFYAN WALA MUSIC
    Sound sfx4 = LoadSound("resources/StopIt.wav");

    Sound sfx5 = LoadSound("resources/woosh.wav");
    Sound sfx6 = LoadSound("resources/randomsfx1.wav");
    Sound sfx7 = LoadSound("resources/randomsfx2.wav");

    Sound gameover = LoadSound("resources/GameOver.wav");

    Sound killSound = LoadSound("resources/killSound.wav");
};

class HealthBar
{
private:
public:
    // Attributes
    Rectangle outerRect;
    Rectangle innerRect;
    Color outerColor;
    Color innerColor;
    int currentHealth;
    Texture2D heartTexture;
};

// Global Functions
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
    /*-----------------------------------------Basic Health Bar-----------------------------------------*/
    // SIMPLE DRAWING OF HEALTH BAR
    //  // Draw outer rectangle
    //  DrawRectangleRec(bar.outerRect, bar.outerColor);

    // // Calculate inner rectangle width based on current health
    // float innerWidth = (bar.currentHealth / (float)maxHealth) * bar.innerRect.width;
    // bar.innerRect.width = (innerWidth < 0) ? 0 : innerWidth;

    // // Draw inner rectangle
    // DrawRectangleRec(bar.innerRect, bar.innerColor);
    /*---------------------------------------------------------------------------------------------*/

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
Enemy InitEnemy(Player p)
{
    // Load the boss enemy sfx
    Sound sfx1 = LoadSound("resources/sfx1edited.wav");
    Sound sfx2 = LoadSound("resources/poinkwav.wav");
    char *texture;
    // BOSS HAVE TO SPAWN ONCE LOGIC
    bool boss;
    if (flag == 0)
    {
        flag = 1;
        // BOSS ENEMY WILL SPAWN ONLY ONCE :D
        texture = const_cast<char *>("media/enemy3.1.png");
        PlaySound(sfx1);
        boss == true; // Set boss enemy speed to 3.0 which is max an enemy can have
    }
    // Randomly choose between enemy1 and enemy2 textures
    else if (GetRandomValue(0, 1) == 0)
    {
        texture = const_cast<char *>("media/enemy1.png");
        PlaySound(sfx2);
        boss = false;
    }
    else
    {
        texture = const_cast<char *>("media/enemy3.png");
        PlaySound(sfx2);
        boss = false;
    }
    Enemy enemy(p.getx(), p.gety(), texture, boss);
    return enemy;
}
void SaveToFile(float score)
{
    std::ofstream outputFile("scores.txt", std::ios::app); // Open the file in append mode

    if (outputFile.is_open())
    {
        outputFile << score << std::endl; // Write the score to the file on a new line
        outputFile.close();               // Close the file
    }
    else
    {
        std::cout << "Failed to open the file for writing." << std::endl;
    }
}

void ShowHighScore()
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "2D Space Game");
    Camera2D camera = {0};
    camera.offset = Vector2({screenWidth / 2.0f, screenHeight / 2.0f});
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    SetTargetFPS(60);                      // Set our game to run at 60 frames-per-second
    std::ifstream inputFile("scores.txt"); // Open the file for reading
    std::vector<int> topScores;            // Vector to store the top scores
    std::string highScores;                // String to store all the high scores
    int topScore = 0;                      // Variable to store the top score
    InitAudioDevice();
    Sound bgmusicforhighscorescreen = LoadSound("resources/highscore.wav");
    PlaySound(bgmusicforhighscorescreen);
    SetSoundVolume(bgmusicforhighscorescreen, 2.6f);
    if (inputFile.is_open())
    {

        std::string score;
        while (std::getline(inputFile, score)) // Read each line from the file
        {
            // Convert the score to an integer
            int scoreValue = std::stoi(score);
            topScores.push_back(scoreValue);
        }
        inputFile.close(); // Close the file

        // Sort the scores in descending order
        sort(topScores.begin(), topScores.end(), std::greater<int>());

        // Resize the vector to contain only the top 5 scores
        int NumberOfScores = 5;

        if (topScores.size() > NumberOfScores)
        {
            topScores.resize(NumberOfScores);
        }
    }
    else
    {
        std::cout << "Failed to open the file for reading." << std::endl;
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

        /*-----------------------------------------Printing the complete file.-----------------------------------------*/
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
        /*-------------------------------------------------------------------------------------------------------------*/

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
void RunGame()
{
    InitWindow(screenWidth, screenHeight, "2D Space Game");
    HealthBar healthBar = CreateHealthBar(50, 50, 200, 30, WHITE, RED, maxHealth);
    InitAudioDevice();
    Vector2 playerVelocity = {0.0f, 0.0f};
    Player player("media/space23.png", "resources/bgmusicwav.wav", "media/space2.png");

    srand(time(NULL));
    std::vector<Enemy> enemies;
    bool restartRequested = false; // Flag to track if restart has been requested.
    PlaySound(player.get_bgMusic());
    SetSoundVolume(player.get_bgMusic(), 0.6f);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    dValues d; // Object to call the d values.

    while (!WindowShouldClose())
    {
        if (!player.get_gameover()) // Only update the game if it's not over
        {
            player.scoreinc(2 * GetFrameTime());
            // Update player movement
            float targetSpeedX = 0.0f;
            float targetSpeedY = 0.0f;

            if (IsKeyDown(KEY_RIGHT) && player.get_x() < boundaryRight)
            {
                targetSpeedX += d.acceleration;
            }
            else if (IsKeyDown(KEY_LEFT) && player.get_x() > boundaryLeft)
            {
                targetSpeedX -= d.acceleration;
            }

            if (IsKeyDown(KEY_DOWN) && player.get_y() < boundaryBottom)
            {
                targetSpeedY += d.acceleration;
            }
            else if (IsKeyDown(KEY_UP) && player.get_y() > boundaryTop)
            {
                targetSpeedY -= d.acceleration;
            }

            if (IsKeyDown(KEY_F))
            {
                // Toggle BOOSTERS
                // boostersActivated = true;
                // PlaySound(sfx5);

                // targetSpeedX *= 25.0f;
                // targetSpeedY *= 25.0f;
                // SetSoundVolume(sfx5, 3.9f);
            }

            // Smoothly accelerate/decelerate towards target speed
            if (targetSpeedX > playerVelocity.x)
            {
                playerVelocity.x = fminf(playerVelocity.x + d.acceleration, d.maxSpeed);
            }
            else if (targetSpeedX < playerVelocity.x)
            {
                playerVelocity.x = fmaxf(playerVelocity.x - d.acceleration, -d.maxSpeed);
            }
            else
            {
                playerVelocity.x *= d.deceleration;
            }

            if (targetSpeedY > playerVelocity.y)
            {
                playerVelocity.y = fminf(playerVelocity.y + d.acceleration, d.maxSpeed);
            }
            else if (targetSpeedY < playerVelocity.y)
            {
                playerVelocity.y = fmaxf(playerVelocity.y - d.acceleration, -d.maxSpeed);
            }
            else
            {
                playerVelocity.y *= d.deceleration;
            }

            // Update player position based on velocityy
            player.setpos(playerVelocity.x, playerVelocity.y);

            // Spawn enemies randomly and limit the number of enemies
            if (GetRandomValue(0, 100) < 1 && enemies.size() < 5) // Adjust spawn rate and max enemies as needed
            {
                enemies.push_back(Enemy(InitEnemy(player)));
            }
            // Update enemy positions
            for (size_t i = 0; i < enemies.size(); i++)
            {
                // Move enemies towards the player (You can update this logic as per your requirement)
                // float randomFloat = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
                Vector2 direction = {player.get_x() - enemies[i].get_x(), player.get_y() - enemies[i].get_y()};
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

                enemies[i].setpos(direction.x, direction.y);

                // Check for collision with player
                //   Rectangle playerRect = {player.get_x() + 40, player.get_y() + 30, player.width - 35, player.height + 30};
                //   Rectangle enemyRect = {enemies[i].position.x, enemies[i].position.y + 20, static_cast<float>(enemies[i].texture.width) - 25, static_cast<float>(enemies[i].texture.height) - 10};
                if (CheckCollisionRecs(player.getrect(), enemies[i].getrect()))
                {
                    PlaySound(d.gameover);

                    player.Gameover(); // Game over if collision detected
                    SaveToFile(player.getscore());
                    break;
                }
                if (CheckCollisionRecs(player.getbulletrect(), enemies[i].getrect()))
                {
                    PlaySound(d.killSound);
                    enemies[i].setstatus();
                    enemies.erase(enemies.begin() + i);
                    player.scoreinc(10);
                }
            }
        }
    }

    return;
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

void ShowMainMenu()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Space Shooter - Main Menu");

    // Load the background image
    Texture2D backgroundImage = LoadTexture("media\\bgimage1600main.png");

    // Adjust the background image rectangle to cover the entire window
    Rectangle bgRec = {0, 0, (float)screenWidth, (float)screenHeight};

    Rectangle playButton = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 100), 100, 50};
    Rectangle highScoreButton = {(float)(screenWidth / 2 + 10), (float)(screenHeight / 2 - 100), 140, 50};
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
        DrawText("Play", (int)playButton.x + 30, (int)playButton.y + 15, 20, WHITE);

        // Draw high score button
        DrawRectangleRec(highScoreButton, GREEN);
        DrawText("High Score", (int)highScoreButton.x + 10, (int)highScoreButton.y + 15, 20, WHITE);

        // Draw about button
        DrawRectangleRec(aboutButton, RED);
        DrawText("About", (int)aboutButton.x + 25, (int)aboutButton.y + 15, 20, WHITE);

        // Draw game name
        DrawText("SPACE SHOOTER GAME", screenWidth / 2 - MeasureText("SPACE SHOOTER GAME", 32) / 2, (screenHeight / 2) + 55, 32, WHITE);
        DrawText("Developed By:\n\nSaim\n\nSufyan\n\nTalha", screenWidth / 2 - MeasureText("Developed By:\n\nSaim\n\nSufyan\n\nTalha", 26) / 2, (screenHeight / 2) + 100, 26, RED);

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

int main()
{
    ShowMainMenu();
    return 0;
}