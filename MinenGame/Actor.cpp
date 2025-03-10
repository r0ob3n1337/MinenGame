#include "Actor.h"

Actor::Actor(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH)
    : renderer(renderer), x(startX), y(startY), textureFrameX(textureFrameX), textureFrameY(textureFrameY), textureFrameW(textureFrameW), textureFrameH(textureFrameH) {
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
            textureFrameX, textureFrameY, textureFrameW, textureFrameH
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
    return { x, y, 32, 32 }; // Размер спрайта 32x32
}

bool Actor::intersects(const Actor& other) const {
    SDL_Rect rect1 = getRect();
    SDL_Rect rect2 = other.getRect();
    return SDL_HasIntersection(&rect1, &rect2);
}