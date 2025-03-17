#include "Bomb.h"
#include <iostream>

const SpriteSettings Bomb::DEFAULT_PLAYER_SETTINGS = { 32, 32, 60, 60 };

Bomb::Bomb(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings)
    : Actor(renderer, texturePath, startX, startY, spriteSettings) {
}

void Bomb::explode() {
    std::cout << "Boom! Bomb at (" << x << ", " << y << ") exploded!" << std::endl;
}