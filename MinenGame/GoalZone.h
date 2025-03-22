#pragma once

#include <SDL2/SDL.h>

class GoalZone {
private:
	SDL_Renderer* renderer;
	SDL_Rect rect;
public:
	GoalZone(SDL_Renderer* renderer, int x, int y, int size);

	void render() const;
	SDL_Rect getRect() const;
};