#include <SDL.h>
#include <iostream>
#include <last-outpost/const.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

int main(int argc, char *args[])
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

    Game::Map map(TILES_X, TILES_Y);
    map.loadFromString(Game::rawStringMap, TILES_Y, TILES_X);

    Game::Graphics graphics;
    graphics.setResolution(SCREEN_WIDTH, SCREEN_HEIGHT, TILES_X, TILES_Y);
    graphics.setRenderer(renderer);

    bool running = true;
    SDL_Event event;
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

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}