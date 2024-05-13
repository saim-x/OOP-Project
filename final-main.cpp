#include "include/raylib.h"
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class SpaceObjects
{
private:
    // Attributes
    Texture2D image_;
    Vector2 position_;

public:
    // Constructors
    SpaceObjects(const Texture2D image, const Vector2 position) : image_(image)
    {
        }
};