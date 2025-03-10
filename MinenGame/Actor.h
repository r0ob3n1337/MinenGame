#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <iostream>

struct SpriteSettings {
    int frameX = 0;
    int frameY = 0;
    int frameW = 32;
    int frameH = 32;
};

class Actor {
protected:
    int x, y;
    SpriteSettings spriteSettings;
    SDL_Texture* texture;
    SDL_Renderer* renderer;

public:
    Actor(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings);
    virtual ~Actor();

    virtual void render() const;
    int getX() const;
    int getY() const;
    SDL_Rect getRect() const;
    bool intersects(const Actor& other) const;
};
