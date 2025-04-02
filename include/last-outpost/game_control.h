#ifndef __LAST_OUTPOST_GAME_CONTROL_H__
#define __LAST_OUTPOST_GAME_CONTROL_H__

#include <SDL.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <last-outpost/level.h>
#include <vector>

namespace Game
{
    class GameControl
    {
    public:
        GameControl(SDL_Renderer *renderer, int screenWidth, int screenHeight, const Level &level);
        void run();

    private:
        SDL_Renderer *renderer;
        Graphics graphics;
        Map map;
        std::vector<Enemy> activeEnemies;
        const Level &level;
        bool running;
        Uint32 lastSpawnTime;
        int spawnedEnemyCount;

        void handleEvents();
        void update();
        void render();
        void spawnEnemies();
    };
}

#endif