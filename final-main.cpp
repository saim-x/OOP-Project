#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Global Values
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

public:
    // Constructors
    SpaceObjects(const Texture2D image) : image_(image)
    {
        // Seed the random number generator
        srand(static_cast<unsigned int>(time(nullptr)));
        // Generate random x and y coordinates within the specified boundaries
        float position_X = randomFloat(boundaryLeft, boundaryRight);
        float position_Y = randomFloat(boundaryTop, boundaryBottom);
        position_ = {position_X, position_Y};
    }

    // Destructor
    ~SpaceObjects() { UnloadTexture(image_); }

    // Setters
    void set_image(const Texture2D image) { this->image_ = image; }
    void set_position(const Vector2 position) { this->position_ = position; }
};

int main()
{
    std::cout << std::endl;

    std::cout << std::endl;
    return 0;
}