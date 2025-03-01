#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Actor {
public:
    Actor(std::string name, int hp) : name(name), hp(hp) {};

    // const говорит о том, что состояние объекта не изменится
    std::string GetName() const {
        return this->name;
    }

    int GetHp() const {
        return this->hp;
    }

    void TakeDamage(int damage) {
        std::cout << this->name << " get " << damage << " damage!" << std::endl;

        this->hp -= damage;
        if (this->hp < 0) {
            this->hp = 0;
        }
    }

    void PrintStatus() {
        std::cout << this->GetName() << " [" << this->GetHp() << "]" << std::endl;
    }
// поля класса
private:
    std::string name;
    int hp;
};


class Enemy : public Actor {
public:
    Enemy(std::string name, int hp, int damage) : Actor(name, hp), damage(damage) {};

    void Attack(Actor& target) {
        std::cout << this->GetName() << " attacked " << target.GetName() << "!" << std::endl;
        target.TakeDamage(this->damage);
    }

private:
    int damage;
};

void QuitGame(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "[ERROR] SDL INIT: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "[ERROR] IMG INIT: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // create the game window
    SDL_Window* window = SDL_CreateWindow(
        "Minen Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "[ERROR] Create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // create rendered
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "[ERROR] Render: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // keyboard settings
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    // texture of player
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/sheep.png");
    if (!playerTexture) {
        std::cerr << "[ERROR] Load texture: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // main loop
    bool isRunning = true;
    SDL_Event event;

    SDL_Rect rect = { 100, 100, 120, 120 };

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                rect.x = mouseX - rect.w / 2;
                rect.y = mouseY - rect.h / 2;
            }
        }

        if (keyboardState[SDL_SCANCODE_Q]) {
            QuitGame(renderer, window);
            return 0;
        }

        // apply color for next step
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R G B A
        // and fill window by the color
        SDL_RenderClear(renderer);


        // START EXPERIMENTS 

        if (keyboardState[SDL_SCANCODE_W]) rect.y -= 5;
        if (keyboardState[SDL_SCANCODE_S]) rect.y += 5;
        if (keyboardState[SDL_SCANCODE_A]) rect.x -= 5;
        if (keyboardState[SDL_SCANCODE_D]) rect.x += 5;
        

        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderFillRect(renderer, &rect);
        SDL_Rect frameRect = {
            35, 35, 60, 60
        };

        SDL_RenderCopy(renderer, playerTexture, &frameRect, &rect);


        // END EXPERIMENTS

        // main loop code here

        // render frame
        SDL_RenderPresent(renderer);
    }

    // memory free before quit
    QuitGame(renderer, window);

    return 0;
}