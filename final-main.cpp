#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <array> //To create standard array of Space Objects.

// Global Variables
int flag = 0;        // flag to check if boss enemy is spawned or not
int maxHealth = 100; // Maximum health of the player
const int screenWidth = 1600;
const int screenHeight = 850;
const float boundaryLeft = -815.0f;
const float boundaryRight = 1715.0f;
const float boundaryTop = -429.0f;
const float boundaryBottom = 664.0f;

// Global Functions
//  Function to generate a random float within a given range
float randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

class SpaceObjects
{
private:
    // Attributes
    Texture2D image_;
    Vector2 position_;
    static int numofobjects;

public:
    // Constructors
    SpaceObjects()
    {
        image_ = LoadTexture("media/icons8-star-16.png");
        numofobjects++;
        // Seed the random number generator

        // Generate random x and y coordinates within the specified boundaries
        position_.x = GetRandomValue(boundaryLeft,boundaryRight);
        position_.y = GetRandomValue(boundaryTop, boundaryBottom);

    }

    // Destructor
    ~SpaceObjects() { UnloadTexture(image_); }

    // Setters
    void set_image(const Texture2D image) { this->image_ = image; }
    void set_position(const Vector2 position) { this->position_ = position; }

    // Getters
    Texture2D get_image() const { return image_; }
    Vector2 get_position() const { return position_; }
    static int get_numofobjects() { return numofobjects; }

    // Methods
    void draw() { DrawTexture(image_, position_.x,position_.y, WHITE); }
};
int SpaceObjects::numofobjects = 0;

class Game
{
protected:
    bool gameover;
    Camera2D camera;
    const char *background;
    Texture2D backgroundtexture;
    Rectangle player;
    const char *texture;
    Texture2D textureobject;
    Vector2 playervelocity;
    float speed;
    const char *music;
    Sound bgMusic;
    float gameTime_;

public:
    Game(const char *texture, const char *music, const char *background) : texture(texture), music(music), background(background), gameTime_(0.0), camera({0})
    { // for player
        speed = 3.0f;
        gameover = false;
        camera.offset = Vector2({screenWidth / 2.0f, screenHeight / 2.0f});
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        backgroundtexture = LoadTexture(background);
        playervelocity.x = 0;
        playervelocity.y = 0;
        player.x = screenWidth / 2;
        player.y = screenHeight / 2;
        player.width = 40;
        player.height = 40;
        textureobject = LoadTexture(texture);
        bgMusic = LoadSound(music);
        BeginMode2D(camera);
    }
    ~Game()
    {
        UnloadTexture(textureobject);
    }
    void setgameover(bool val) { gameover = val; }
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
class dValues
{
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

public:
    dValues() {}
    friend class Player;
};
class Player : public Game
{
protected:
    float score;
    double lastFireTime_;
    dValues d;
public:
    Player(const char *texture, const char *music, const char *background) : Game(texture, music, background)
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
    // Player operator+(Vector2 pos){
    //     Player p;
    //     p.player.x=player.x+pos.x;
    //     p.player.y=player.y+pos.y;
    // }
    void Gameover()
    {
        gameover = true;
        player.x = 0;
        player.y = 0;
    }
    void draw()
    {
        DrawTexture(backgroundtexture, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
        DrawTextureEx(textureobject, Vector2({player.x, player.y}), 0.0f, 1.0f, WHITE);
    }
    ~Player()
    {
        UnloadTexture(backgroundtexture);
        UnloadSound(bgMusic);
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
    Vector2 getpos()
    {
        return Vector2{player.x, player.y};
    }
    // Getter functions for dValues class
    float get_maxSpeed() { return d.maxSpeed; }
    float get_acceleration() { return d.acceleration; }
    float get_deceleration() { return d.deceleration; }
    Sound get_sfx4() const { return d.sfx4; }
    Sound get_sfx5() const { return d.sfx5; }
    Sound get_sfx6() const { return d.sfx6; }
    Sound get_sfx7() const { return d.sfx7; }
    Sound get_gameoversound() const { return d.gameover; }
    Sound get_killSound() const { return d.killSound; }
};


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
    InitAudioDevice();
    Vector2 playerVelocity = {0.0f, 0.0f};
    char *obj = const_cast<char *>("media/spacecraft23.png");
    char *music = const_cast<char *>("resources/bgmusicwav.wav");
    char *bg = const_cast<char *>("media/space2.png");
    Player player(obj, music, bg);
    std::array<SpaceObjects, 30> spobj;
    srand(time(NULL));
    bool restartRequested = false; // Flag to track if restart has been requested.
    PlaySound(player.get_bgMusic());
    SetSoundVolume(player.get_bgMusic(), 0.6f);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    dValues d;        // Object to call the d values.
    
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
                targetSpeedX += player.get_acceleration();
            }
            else if (IsKeyDown(KEY_LEFT) && player.get_x() > boundaryLeft)
            {
                targetSpeedX -= player.get_acceleration();
            }
            if (IsKeyDown(KEY_DOWN) && player.get_y() < boundaryBottom)
            {
                targetSpeedY += player.get_acceleration();
            }
            else if (IsKeyDown(KEY_UP) && player.get_y() > boundaryTop)
            {
                targetSpeedY -= player.get_acceleration();
            }
            if (IsKeyDown(KEY_F))
            {
                // Toggle BOOSTERS
                // boostersActivated = true;
                PlaySound(player.get_sfx5());
                targetSpeedX *= 25.0f;
                targetSpeedY *= 25.0f;
                SetSoundVolume(player.get_sfx5(), 3.9f);
            }
            // Smoothly accelerate/decelerate towards target speed
            if (targetSpeedX > playerVelocity.x)
            {
                playerVelocity.x = fminf(playerVelocity.x + player.get_acceleration(), player.get_maxSpeed());
            }
            else if (targetSpeedX < playerVelocity.x)
            {
                playerVelocity.x = fmaxf(playerVelocity.x - player.get_acceleration(), -player.get_maxSpeed());
            }
            else
            {
                playerVelocity.x *= player.get_deceleration();
            }
            if (targetSpeedY > playerVelocity.y)
            {
                playerVelocity.y = fminf(playerVelocity.y + player.get_acceleration(), player.get_maxSpeed());
            }
            else if (targetSpeedY < playerVelocity.y)
            {
                playerVelocity.y = fmaxf(playerVelocity.y - player.get_acceleration(), -player.get_maxSpeed());
            }
            else
            {
                playerVelocity.y *= player.get_deceleration();
            }

            // Update player position based on velocityy
            player.setpos(playerVelocity.x, playerVelocity.y);
            // Update enemy positions
            BeginDrawing();
            ClearBackground(RAYWHITE);
            player.draw();
            for (int i = 0; i < 30; i++)
            {
                spobj[i].draw();
            }
            EndMode2D();
            DrawText("Space Shooter", 10, 10, 20, RED);
            DrawText(TextFormat("Score: %.2f ", player.getscore()), screenWidth - MeasureText(TextFormat("%.2f seconds", player.getscore()), 20) - 10, 10, 20, WHITE);
            // Draw legend
            DrawText("Arrows: Move", screenWidth - MeasureText("Arrows: Move", 20) - 10, screenHeight - 60, 20, WHITE);
            DrawText("F: Boost", screenWidth - MeasureText("F: Boost", 20) - 10, screenHeight - 30, 20, WHITE);
            DrawText("Exit: Escape", screenWidth - MeasureText("Exit: Escape", 20) - 10, screenHeight - 100, 20, WHITE);

            // Update and draw health bar or enemy counterr
            EndDrawing();
        }
    }
    SaveToFile(player.getscore());
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