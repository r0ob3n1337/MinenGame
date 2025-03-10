#pragma once

#include "Actor.h"
#include "Bomb.h"

#include <vector>

class Player : public Actor {
public:
    Player(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH);
    void handleInput(const Uint8* keyboardState);
    //void checkCollisions(const std::vector<Bomb>& bombs);
};

