#ifndef __LAST_OUTPOST_GAME_CONTROL_H__
#define __LAST_OUTPOST_GAME_CONTROL_H__

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

#endif