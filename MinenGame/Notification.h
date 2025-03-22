#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Notification {
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Rect rect;
public:
	Notification(SDL_Renderer* renderer);
	~Notification();

	// type is WIN or LOSE
	void setText(const std::string& message, const std::string& type);
	void render() const;
	void clear();
};
