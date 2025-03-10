#pragma once

#include "Actor.h"
#include "Bomb.h"

#include <vector>

class Player : public Actor {
public:
    static const SpriteSettings DEFAULT_PLAYER_SETTINGS;

    Player(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings = DEFAULT_PLAYER_SETTINGS);
    void handleInput(const Uint8* keyboardState);
    //void checkCollisions(const std::vector<Bomb>& bombs);
};

