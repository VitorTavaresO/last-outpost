#include <SDL.h>
#include <iostream>
#include <last-outpost/globals.h>
#include <last-outpost/game_control.h>
#include <last-outpost/level.h>

namespace Game
{
    int main(int argc, char **argv)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Error on start SDL: " << SDL_GetError() << std::endl;
            return 1;
        }

        SDL_Window *window = SDL_CreateWindow("View Map", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            std::cerr << "Error on Window Creation " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            std::cerr << "Error on render creation: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        std::vector<Enemy> enemyTypes = {Enemy(100, 20, 5.0f, "Fireball")};

        Level LEVEL_1(rawStringMap, enemyTypes, 100);

        GameControl gameControl(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, LEVEL_1);
        gameControl.run();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 0;
    }
}

//--------------------- end namespace Game ---------------------

int main(int argc, char **argv)
{
    return Game::main(argc, argv);
}