#ifndef __LAST_OUTPOST_GAME_WORLD_H__
#define __LAST_OUTPOST_GAME_WORLD_H__

#include <vector>
#include <memory>
#include <SDL.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <last-outpost/level.h>

namespace Game
{
	class GameWorld
	{
	public:
		GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level);
		void run();

	private:
		SDL_Renderer *renderer;
		Graphics graphics;
		Map map;
		std::vector<std::unique_ptr<Enemy>> activeEnemies;
		Level level;
		bool running;
		uint32_t lastSpawnTime;
		uint32_t lastUpdateTime;
		int spawnedEnemyCount;

		void handleEvents();
		void update();
		void render();
		void spawnEnemies();
	};
}

#endif