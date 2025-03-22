#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include "../Player.h"
#include "../Bomb.h"
#include "../GoalZone.h"
#include "../Notification.h"
#include "../TextButton.h"


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
const SDL_Color YELLOW_COLOR = { 255, 255, 0, 255 };

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;
GoalZone* goalZone = nullptr;
Notification* notification = nullptr;
TextButton* restartButton = nullptr;
TextButton* exitButton = nullptr;

Player* player = nullptr;

std::vector<Bomb*> bombs;

bool allowPlayerMove = false;
bool isEndGame = false;
bool isWin = false;

void closeGame() {
    delete goalZone;
    delete notification;
    delete restartButton;
    delete exitButton;
    delete player;

    for (auto& bomb : bombs) {
        delete bomb;
    }
    bombs.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
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

    TTF_Init();
    notification = new Notification(renderer);

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


void initPlayer() {
    int playerWidth = 60;
    int playerHeight = 60;
    int playerStartX = (WINDOW_WIDTH / 2 / CELL_SIZE) * CELL_SIZE + (CELL_SIZE - playerWidth) / 2;
    int playerStartY = (WINDOW_HEIGHT - CELL_SIZE) + (CELL_SIZE - playerHeight) / 2;

    player = new Player(renderer, "assets/sheep.png", playerStartX, playerStartY);
}


void initGoalZone(SDL_Renderer* renderer, int cellSize, int gridWidth) {
    int randomCellX = (rand() % gridWidth) * cellSize;
    goalZone = new GoalZone(renderer, randomCellX, 0, cellSize);
}


void initBombs(SDL_Renderer* renderer, const std::string& texturePath, int count) {
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

bool playerIsBang() {
    bool isBang = false;

    for (auto& bomb : bombs) {
        if (player->isBang(*bomb)) {
            isBang = true;
            break;
        };
    }

    return isBang;
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

        SDL_SetRenderDrawColor(renderer, 30, 40, 120, 255);
        SDL_RenderClear(renderer);

        renderGrid(renderer, CELL_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);
        renderBombs();

        SDL_RenderPresent(renderer);

        if ((SDL_GetTicks() - startTime) >= seconds * 1000) {
            showing = false;
        }
        SDL_Delay(16);
    }

    allowPlayerMove = true;
}


void restartGame() {
    delete player;
    delete goalZone;
    for (auto& bomb : bombs) {
        delete bomb;
    }
    bombs.clear();

    isEndGame = false;
    isWin = false;
    allowPlayerMove = false;

    initPlayer();
    initGoalZone(renderer, CELL_SIZE, WINDOW_WIDTH / CELL_SIZE);
    initBombs(renderer, "assets/sheep.png", 10);

    showBombsByNSeconds(3);
}


int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return 1;
    }

    srand(time(nullptr));

    initPlayer();
    initGoalZone(renderer, CELL_SIZE, WINDOW_WIDTH / CELL_SIZE);
    initBombs(renderer, "assets/sheep.png", 10);
    showBombsByNSeconds(3);

    // --------------------------
    // ----- MAIN LOOP START ----

    bool isRunning = true;
    SDL_Event event;

    restartButton = new TextButton(renderer, "Try again", 180, 450, YELLOW_COLOR);
    restartButton->setOnClick([]() {
        notification->clear();
        restartGame();
    });

    exitButton = new TextButton(renderer, "Exit", 350, 450, YELLOW_COLOR);
    exitButton->setOnClick([]() {
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    });

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }

            if (allowPlayerMove) {
                player->handleEvent(event, CELL_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);
            }

            restartButton->handleEvent(event);
            exitButton->handleEvent(event);
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[SDL_SCANCODE_Q]) {
            closeGame();
            return 0;
        }

        // apply color for next step + fill window by the color
        SDL_SetRenderDrawColor(renderer, 30, 40, 120, 255); // R G B A
        SDL_RenderClear(renderer);

        if (isEndGame) {
            if (isWin) {
                notification->setText("You are wining, son!", "win");
            }
            else {
                notification->setText("Not for today... Try again!", "lose");
            }

            notification->render();
            restartButton->render();
            exitButton->render();
        }
        else {
            // render goal zone under the Grid
            goalZone->render();

            // render cells for minens
            renderGrid(renderer, 60, WINDOW_WIDTH, WINDOW_HEIGHT);

            // render player
            player->render();


            if (playerIsBang()) {
                std::cout << "Oh no!" << std::endl;
                isEndGame = true;
            }

            if (player->isGoal(*goalZone)) {
                // TODO: restart game logic here
                std::cout << "You are winning son!" << std::endl;
                isWin = true;
                isEndGame = true;
            }
        }

        SDL_RenderPresent(renderer);
    }

    // ----- MAIN LOOP END ------
    // --------------------------

    // memory free before quit
    closeGame();

    return 0;
}
