#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <iostream>

class Actor {
protected:
    int x, y, textureFrameX, textureFrameY, textureFrameW, textureFrameH;
    SDL_Texture* texture;
    SDL_Renderer* renderer;

public:
    Actor(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, int textureFrameX, int textureFrameY, int textureFrameW, int textureFrameH);
    virtual ~Actor();

    virtual void render() const;
    int getX() const;
    int getY() const;
    SDL_Rect getRect() const;
    bool intersects(const Actor& other) const;
};
