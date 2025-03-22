#pragma once

#include "Actor.h"
#include "Bomb.h"
#include "GoalZone.h"

#include <vector>

class Player : public Actor {
public:
    static const SpriteSettings DEFAULT_PLAYER_SETTINGS;

    Player(SDL_Renderer* renderer, const std::string& texturePath, int startX, int startY, const SpriteSettings& spriteSettings = DEFAULT_PLAYER_SETTINGS);
    void handleEvent(const SDL_Event& event, int cellSize, int windowWidth, int windowHeight);
    void move(int deltaX, int deltaY, int windowWidth, int windowHeight, int cellSize);
    bool isGoal(const GoalZone& goalZone) const;
    //void checkCollisions(const std::vector<Bomb>& bombs);
};

