#include "Bomb.h"
#include <iostream>

Bomb::Bomb(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings)
    : Actor(renderer, texturePath, startX, startY, spriteSettings) {
}

void Bomb::explode() {
    std::cout << "Boom! Bomb at (" << x << ", " << y << ") exploded!" << std::endl;
}