#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

    const int screenWidth = 1600;
    const int screenHeight = 850;

    class Game{
        Rectangle player;
        char* texture;
        Texture2D textureobject;
        Vector2 playervelocity;
        const float maxSpeed = 26.0f;    // Adjusted maximum speed
        const float acceleration = 3.0f; // Adjusted acceleration
        const float deceleration = 1.0f;
        char* music;
        Sound bgMusic;
        public:
        Game(char* texture, char* music):texture(texture),music(music){
            playervelocity.x=0.0f;
            playervelocity.y=0.0f;
            player.x=0;
            player.y=0;
            player.width=40;
            player.height=40;
            textureobject=LoadTexture(texture);
            bgMusic= LoadSound(music);
        }
        ~Game(){
            UnloadTexture(textureobject);
            UnloadSound(bgMusic);
        }
    };
