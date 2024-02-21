#pragma once

#include <iostream>
#include <string>

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
using namespace std;

//Used for every element on the screen. Combines the two things needed to siplay sth - texture and rect
struct Drawable
{
    SDL_Texture* texture = nullptr;
    SDL_Rect rect = { 0 }; /// The rect where we draw
};
