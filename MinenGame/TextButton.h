#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>


class TextButton {
private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Rect rect;
	SDL_Color color;
	std::string text;
	std::function<void()> onClick;
public:
	TextButton(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
	~TextButton();

	void render() const;
	void handleEvent(const SDL_Event& event);
	void setOnClick(std::function<void()> callback);
};

