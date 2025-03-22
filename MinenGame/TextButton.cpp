#include "TextButton.h"
#include <iostream>


TextButton::TextButton(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color)
    : renderer(renderer), font(nullptr), texture(nullptr), text(text), color(color) {

    font = TTF_OpenFont("assets/Roboto-Bold.ttf", 28);
    if (!font) {
        std::cerr << "Failed to load font for button: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create button surface: " << TTF_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create button texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    rect.w = surface->w;
    rect.h = surface->h;
    rect.x = x;
    rect.y = y;

    SDL_FreeSurface(surface);
}


TextButton::~TextButton() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
}


void TextButton::render() const {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}


void TextButton::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // check the click position
        // click must be inside the button rect size
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && 
            mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            if (onClick) {
                onClick();
            }
        }
    }
}


void TextButton::setOnClick(std::function<void()> callback) {
    onClick = callback;
}