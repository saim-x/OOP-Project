#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

    const int screenWidth = 1600;
    const int screenHeight = 850;

    class Game{
        protected:
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
        Game(char* texture, char* music):texture(texture),music(music){//for player
            playervelocity.x=0.0f;
            playervelocity.y=0.0f;
            player.x=0;
            player.y=0;
            player.width=40;
            player.height=40;
            textureobject=LoadTexture(texture);
            bgMusic= LoadSound(music);
        }
        Game(float x, float y, char* texture, char* music):texture(texture),music(music){
            player.x=x;
            player.y=y;
            textureobject=LoadTexture(texture);
            player.height=textureobject.height;
            player.width=textureobject.width;
        }
        ~Game(){
            UnloadTexture(textureobject);
            UnloadSound(bgMusic);
        }
        virtual void setpos(float x, float y)=0;
        float getx(){return player.x;}
        float gety(){return player.y;}
    };

    class Player:public Game{
        protected:
        float score;
        public:
        Player(char* texture, char* music):Game(texture,music){
            score=0;
        }
    };

    class Enemy:public Game{
        float speed;
        public:
            Enemy(float x, float y, char* texture, char* music):Game(texture,music){
                speed = GetRandomValue(15, 30) / 10.0f; // Set enemy speed randomly from 1.5 to 3.0
            }
            void setpos(float x, float y){
            player.x=x;
            player.y=y;
        }
    };
