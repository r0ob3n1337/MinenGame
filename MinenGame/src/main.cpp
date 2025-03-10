#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Player.h"
#include "../Bomb.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;

void closeGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[ERROR] SDL INIT: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "[ERROR] IMG INIT: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(
        "Minen Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "[ERROR] Create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "[ERROR] Render: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    playerTexture = IMG_LoadTexture(renderer, "assets/sheep.png");
    if (!playerTexture) {
        std::cerr << "[ERROR] Load texture: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return 1;
    }


    Player player(renderer, "assets/sheep.png", 100, 100);
    Bomb bomb1(renderer, "assets/sheep.png", 500, 500);

    // main loop
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[SDL_SCANCODE_Q]) {
            closeGame();
            return 0;
        }
        player.handleInput(keyboardState);

        // apply color for next step + fill window by the color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R G B A
        SDL_RenderClear(renderer);

        // render player
        player.render();

        bomb1.render();

        // render frame
        SDL_RenderPresent(renderer);
    }

    // memory free before quit
    closeGame();

    return 0;
}
