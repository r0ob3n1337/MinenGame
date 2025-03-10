#include "Actor.h"

Actor::Actor(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings)
    : renderer(renderer), x(startX), y(startY), spriteSettings(spriteSettings) {
    texture = IMG_LoadTexture(renderer, texturePath.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
}

Actor::~Actor() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Actor::render() const {
    if (texture) {
        SDL_Rect frameRect = { 
            spriteSettings.frameX, spriteSettings.frameY,
            spriteSettings.frameW, spriteSettings.frameH,
        };

        SDL_Rect destRect = { x, y, 60, 60 };
        SDL_RenderCopy(renderer, texture, &frameRect, &destRect);
    }
}

int Actor::getX() const {
    return x;
}

int Actor::getY() const {
    return y;
}

SDL_Rect Actor::getRect() const {
    return { x, y, 60, 60 }; // sprite size 60x60
}

bool Actor::intersects(const Actor& other) const {
    SDL_Rect rect1 = getRect();
    SDL_Rect rect2 = other.getRect();
    return SDL_HasIntersection(&rect1, &rect2);
}