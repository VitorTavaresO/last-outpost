#include <SDL.h>
#include <iostream>
#include <last-outpost/globals.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>

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

        constexpr int TILES_X = 16;
        constexpr int TILES_Y = 9;

        Map map(TILES_X, TILES_Y, rawStringMap);

        Graphics graphics(SCREEN_WIDTH, SCREEN_HEIGHT, TILES_X, TILES_Y, renderer);

        bool running = true;
        SDL_Event event;

        Enemy enemy(100, 20, 2.0f, "Fireball");
        enemy.setSize(1, 1);

        auto path = map.extractPath();
        bool initialized = false;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            map.render(graphics);

            if (!initialized && !path.empty())
            {
                enemy.setPosition(path[0].first, path[0].second);
                initialized = true;
            }

            if (enemy.followPath(path))
            {
                enemy.render(graphics);
            }

            SDL_RenderPresent(renderer);
        }

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