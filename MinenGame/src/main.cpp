#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Player.h"
#include "../Bomb.h"
#include <unordered_set>

/**

Игра «Минное поле».

Правила игры. На экране, расчерченном на клетки, 
произвольным образом расставлены мины. Изображение 
расставленных мин на несколько секунд появляется на экране, 
а затем удаляется. Игрок должен пройти через минное поле из
заданной начальной точки в указанную конечную, не задев ни одной мины.

*/

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 60;


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;

std::vector<Bomb*> bombs;


void closeGame() {
    for (auto& bomb : bombs) {
        delete bomb;
    }
    bombs.clear();

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


void renderGrid(SDL_Renderer* renderer, int cellSize, int width, int height) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i <= width; i += cellSize) {
        SDL_RenderDrawLine(renderer, i, 0, i, height);
    }

    for (int j = 0; j <= height; j += cellSize) {
        SDL_RenderDrawLine(renderer, 0, j, width, j);
    }
}


void initBombs(SDL_Renderer* renderer, const std::string& texturePath, int count) {
    srand(time(nullptr));
    std::unordered_set<int> occupiedCells;

    while (bombs.size() < count) {
        int cell = rand() % 100; // 0-99
        if (occupiedCells.find(cell) == occupiedCells.end()) {
            occupiedCells.insert(cell);
            int x = (cell % 10) * CELL_SIZE;
            int y = (cell / 10) * CELL_SIZE;

            std::cout << "bomb x: " << x << "; y:  " << y << std::endl;

            Bomb* bomb = new Bomb(renderer, texturePath, x, y);
            bombs.push_back(bomb);
        }
    }
}


void renderBombs() {
    for (auto& bomb : bombs) {
        bomb->render();
    }
}


void showBombsByNSeconds(int seconds) {
    Uint32 startTime = SDL_GetTicks();
    bool showing = true;

    while (showing) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                showing = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderGrid(renderer, CELL_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);
        renderBombs();

        SDL_RenderPresent(renderer);

        if (SDL_GetTicks() - startTime >= seconds * 1000) {
            showing = false;
        }
        SDL_Delay(16);
    }
}


int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return 1;
    }

    Player player(renderer, "assets/sheep.png", 100, 100);

    initBombs(renderer, "assets/sheep.png", 10);
    showBombsByNSeconds(3);

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

        // render cells for minens
        renderGrid(renderer, 60, WINDOW_WIDTH, WINDOW_HEIGHT);

        // render player
        player.render();

        // render frame
        SDL_RenderPresent(renderer);
    }

    // memory free before quit
    closeGame();

    return 0;
}
