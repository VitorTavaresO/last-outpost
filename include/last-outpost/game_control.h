#pragma once

#include <SDL.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>

namespace Game
{
    class GameControl
    {
    public:
        GameControl(SDL_Renderer *renderer, int screenWidth, int screenHeight);
        void run();

    private:
        SDL_Renderer *renderer;
        Graphics graphics;
        Map map;
        Enemy enemy;
        bool running;

        void handleEvents();
        void update();
        void render();
    };
}