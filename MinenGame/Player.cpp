#include "Player.h"
#include "Bomb.h"
#include "GoalZone.h"

const SpriteSettings Player::DEFAULT_PLAYER_SETTINGS = { 32, 32, 60, 60 };

Player::Player(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings)
    : Actor(renderer, texturePath, startX, startY, spriteSettings) {
}

void Player::handleEvent(const SDL_Event& event, int cellSize, int windowWidth, int windowHeight) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            std::cout << event.key.keysym.sym << std::endl;
            move(0, -cellSize, windowWidth, windowHeight, cellSize);
            break;
        case SDLK_s:
            move(0, cellSize, windowWidth, windowHeight, cellSize);
            break;
        case SDLK_a:
            move(-cellSize, 0, windowWidth, windowHeight, cellSize);
            break;
        case SDLK_d:
            move(cellSize, 0, windowWidth, windowHeight, cellSize);
            break;
        }
    }
}

void Player::move(int deltaX, int deltaY, int windowWidth, int windowHeight, int cellSize) {
    int newX = x + deltaX;
    int newY = y + deltaY;

    if (newX >= 0 && newX <= windowWidth - cellSize) {
        x = newX;
    }
    if (newY >= 0 && newY <= windowHeight - cellSize) {
        y = newY;
    }
}


bool Player::isGoal(const GoalZone& goalZone) const {
    // use variables for getting values
    // and then use pointers to this values for SDL_HasIntersection
    // because our getRect methods returns values, but not pointers
    // Some workaround :)
    SDL_Rect playerRect = getRect();
    SDL_Rect goalRect = goalZone.getRect();
    return SDL_HasIntersection(&playerRect, &goalRect);
}


//void Player::checkCollisions(const std::vector<Bomb>& bombs) {
//    for (const auto& bomb : bombs) {
//        if (intersects(bomb)) {
//            std::cout << "Player hit a bomb at (" << bomb.getX() << ", " << bomb.getY() << ")!" << std::endl;
//            bomb.explode();
//        }
//    }
//}
