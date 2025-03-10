#include "Player.h"
#include "Bomb.h"

Player::Player(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH)
    : Actor(renderer, texturePath, startX, startY, textureFrameX, textureFrameY, textureFrameW, textureFrameH) {
}

void Player::handleInput(const Uint8* keyboardState) {
    if (keyboardState[SDL_SCANCODE_W] && y > 0) {
        y -= 5;
    }
    if (keyboardState[SDL_SCANCODE_S] && y < 600 - 32) {
        y += 5;
    }
    if (keyboardState[SDL_SCANCODE_A] && x > 0) {
        x -= 5;
    }
    if (keyboardState[SDL_SCANCODE_D] && x < 800 - 32) {
        x += 5;
    }
}

//void Player::checkCollisions(const std::vector<Bomb>& bombs) {
//    for (const auto& bomb : bombs) {
//        if (intersects(bomb)) {
//            std::cout << "Player hit a bomb at (" << bomb.getX() << ", " << bomb.getY() << ")!" << std::endl;
//            bomb.explode();
//        }
//    }
//}
