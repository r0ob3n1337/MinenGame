#include "GoalZone.h"

GoalZone::GoalZone(SDL_Renderer* renderer, int x, int y, int size)
    : renderer(renderer) {
    rect = { x, y, size, size };
}

void GoalZone::render() const {
    // green zone for our "safe zone" or "goal zone"
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect GoalZone::getRect() const {
    return rect;
}