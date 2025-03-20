#include <SDL.h>
#include <iostream>
#include <last-outpost/map.h>

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 800;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Erro ao inicializar a SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Mapa Visual", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Erro ao criar a janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Erro ao criar o renderizador: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int cellWidth = SCREEN_WIDTH / MAP_WIDTH;
    int cellHeight = SCREEN_HEIGHT / MAP_HEIGHT;

    Game::Map map(MAP_WIDTH, MAP_HEIGHT);

    for (int row = 0; row < MAP_HEIGHT; ++row)
    {
        for (int col = 0; col < MAP_WIDTH; ++col)
        {
            if ((row + col) % 2 == 0)
            {
                map.setTerrainValue(row, col, 0);
            }
            else
            {
                map.setTerrainValue(row, col, 1);
            }
        }
    }

    // Loop principal
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

        map.render(renderer, cellWidth, cellHeight);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}