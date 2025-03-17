#pragma once

#include "Actor.h"

class Bomb : public Actor {
public:
    static const SpriteSettings DEFAULT_PLAYER_SETTINGS;

    Bomb(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings = DEFAULT_PLAYER_SETTINGS);
    void explode();
};