#include "Bomb.h"
#include <iostream>

Bomb::Bomb(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH)
    : Actor(renderer, texturePath, startX, startY, textureFrameX, textureFrameY, textureFrameW, textureFrameH) {
}

void Bomb::explode() {
    std::cout << "Boom! Bomb at (" << x << ", " << y << ") exploded!" << std::endl;
}