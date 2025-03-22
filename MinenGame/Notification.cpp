#include <iostream>
#include "Notification.h"

Notification::Notification(SDL_Renderer* renderer)
	: renderer(renderer), font(nullptr), texture(nullptr) {
	font = TTF_OpenFont("assets/Roboto-Bold.ttf", 32);

	if (!font) {
		std::cerr << "[ERROR] Failed to load font: " << TTF_GetError() << std::endl;
	}
}


Notification::~Notification() {
	if (texture) {
		SDL_DestroyTexture(texture);
	}

	if (font) {
		TTF_CloseFont(font);
	}
}


void Notification::setText(const std::string& message, const std::string& type) {
	if (texture) {
		// reset texture
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	SDL_Color color;

	if (type == "win") {
		color = { 0, 200, 0, 255 };
	}
	else if (type == "lose") {
		color = { 200, 0, 0, 255 };
	}
	else {
		color = { 200, 200, 200, 255 };
	}

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, message.c_str(), color);
	if (!surface) {
		std::cerr << "[ERROR] Failed to create surface of font: " << TTF_GetError() << std::endl;
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "[ERROR] Failed to create texture from surface: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return;
	}

	// show text in to the windod center
	rect.w = surface->w;
	rect.h = surface->h;
	rect.x = (600 - rect.w) / 2;
	rect.y = (600 - rect.h) / 2 - 40;

	SDL_FreeSurface(surface);
}

void Notification::render() const {
	if (texture) {
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	}
}

void Notification::clear() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}