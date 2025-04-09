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
		float lastSpawnTime;
		float lastUpdateTime;
		int spawnedEnemyCount;
		size_t enemyTypeIndex;

		void handleEvents();
		void update(float deltaTime);
		void render(float deltaTime);
		void spawnEnemies();
	};
}

#endif