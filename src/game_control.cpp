#include <last-outpost/game_control.h>

namespace Game
{
    constexpr int TILES_X = 32;
    constexpr int TILES_Y = 18;
    GameControl::GameControl(SDL_Renderer *renderer, int screenWidth, int screenHeight)
        : renderer(renderer),
          graphics(screenWidth, screenHeight, TILES_X, TILES_Y, renderer),
          map(TILES_X, TILES_Y, rawStringMap),
          enemy(100, 20, 1.0f, "Fireball"),
          running(true)
    {
        enemy.setSize(1, 1);
        auto path = map.extractPath();
        if (!path.empty())
        {
            enemy.setPosition(path[0].first, path[0].second);
        }
    }

    void GameControl::run()
    {
        while (running)
        {
            handleEvents();
            update();
            render();
        }
    }

    void GameControl::handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }

    void GameControl::update()
    {
        auto path = map.extractPath();
        enemy.update(path);
    }

    void GameControl::render()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        map.render(graphics);
        enemy.render(graphics);

        SDL_RenderPresent(renderer);
    }
}