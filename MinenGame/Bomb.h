#pragma once

#include "Actor.h"

class Bomb : public Actor {
public:
    Bomb(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH);
    void explode();
};