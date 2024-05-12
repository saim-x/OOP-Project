#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

const int screenWidth = 1600;
const int screenHeight = 850;

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
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;
    char *music;
    Sound bgMusic;

public:
    Game(char *texture, char *music, char *background) : texture(texture), music(music), background(background)
    { // for player
        gameover = false;
        Vector2 offSet_Camera = {screenWidth / 2.0f, screenHeight / 2.0f};
        camera.offset = offSet_Camera;
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
        player.x = x;
        player.y = y;
        textureobject = LoadTexture(texture);
        player.height = textureobject.height;
        player.width = textureobject.width;
        Vector2 playingPosition_ = {player.x, player.y};
        DrawTextureEx(textureobject, playingPosition_, 0.0f, 1.0f, WHITE);
    }
    void drawplayer()
    {
        BeginMode2D(camera);
        DrawTexture(backgroundtexture, -static_cast<float>(screenWidth) / 2 - camera.target.x, -static_cast<float>(screenHeight) / 2 - camera.target.y, WHITE);
        Vector2 playingPosition_ = {player.x, player.y};
        DrawTextureEx(textureobject, playingPosition_, 0.0f, 1.0f, WHITE);
    }
    void drawenemy()
    {
        Vector2 playingPosition_ = {player.x, player.y};
        DrawTextureEx(textureobject, playingPosition_, 0.0f, 1.0f, WHITE);
    }
    ~Game()
    {
        UnloadTexture(textureobject);
        UnloadSound(bgMusic);
    }
    virtual void setpos(float x, float y) = 0;
    float getx() { return player.x; }
    float gety() { return player.y; }
    float getwidth() { return player.height; }
    float getheight() { return player.width; }
    bool gameover() { return gameover; }
    Rectangle getrect() { return player; }
};

class Player : public Game
{
protected:
    float score;

    // For Bullets
    double lastFireTime_;

public:
    Player(char *texture, char *music, char *background) : Game(texture, music, background)
    {
        score = 0;
        lastFireTime_ = 0.0;
    }
    void setpos(float x, float y)
    {
        // We can try operator overloading here.
        player.x += x;
        player.y += y;
    }
    void gameover()
    {
        player.x = 0;
        player.y = 0;
    }

    // For Bullets
    void FireLaser()
    {
        if (GetTime() - lastFireTime_ >= 0.35)
        {
            bullets.push_back(Bullet({player.x, player.y}, -6));
            lastFireTime_ = GetTime();
        }
    }
    std::vector<Bullet> bullets;
};

class Enemy : public Game
{
    float speed;

public:
    Enemy(float x, float y, char *texture) : Game(x, y, texture)
    {
        speed = GetRandomValue(15, 30) / 10.0f; // Set enemy speed randomly from 1.5 to 3.0
    }
    void setpos(float x, float y)
    {
        player.x += x * speed;
        player.y += y * speed;
    }
};

class DefaultValues
{
private:
public:
    // These values will be global.
    //  const int screenWidth = 1600;
    //  const int screenHeight = 850;

    Rectangle player = {0, 0, 40, 40};
    Vector2 playerVelocity = {0.0f, 0.0f};
    const float maxSpeed = 26.0f;    // Adjusted maximum speed
    const float acceleration = 3.0f; // Adjusted acceleration
    const float deceleration = 1.0f;

    const float boundaryLeft = -815.0f;
    const float boundaryRight = 715.0f;
    const float boundaryTop = -429.0f;
    const float boundaryBottom = 332.0f;

    Sound bgMusic = LoadSound("resources/bgmusicwav.wav"); // SUFYAN WALA MUSIC
    Sound sfx4 = LoadSound("resources/StopIt.wav");

    Sound sfx5 = LoadSound("resources/woosh.wav");
    Sound sfx6 = LoadSound("resources/randomsfx1.wav");
    Sound sfx7 = LoadSound("resources/randomsfx2.wav");

    Sound gameover = LoadSound("resources/GameOver.wav");
};

class Bullet
{
private:
    // Attributes
    Vector2 position_;
    const int speed_;

public:
    // Attributes
    bool active_; // Variable to check if bullet is still within the game window.

    //  Constructors
    Bullet(const Vector2 position, const int speed) : position_(position), speed_(speed), active_(true) {}

    // Methods
    // Function to update the bullet's position.
    void Update()
    {
        if (IsKeyPressed(KEY_W))
        {
            position_.y += speed_;
        }
        else if (IsKeyPressed(KEY_A))
        {
            position_.x -= speed_;
        }
        else if (IsKeyPressed(KEY_D))
        {
            position_.x += speed_;
        }
        else if (IsKeyPressed(KEY_S))
        {
            position_.y -= speed_;
        }

        if (active_)
        {
            if (position_.y > GetScreenHeight() - 100 || position_.y < 25 || position_.x > GetScreenWidth() - 100 || position_.x < 25)
            {
                active_ = false;
            }
        }
    }
    // Function to draw the bullet.
    void Draw()
    {
        if (active_)
        {
            DrawRectangle(position_.x, position_.y, 4, 15, {243, 216, 63, 255});
        }
        return;
    }
};