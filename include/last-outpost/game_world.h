#ifndef LAST_OUTPOST_GAME_WORLD_H
#define LAST_OUTPOST_GAME_WORLD_H

#include <vector>
#include <memory>
#include <SDL.h>
#include <last-outpost/map.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <last-outpost/tower.h>
#include <last-outpost/projectil.h>
#include <last-outpost/level.h>

namespace Game
{
	class GameWorld
	{
	public:
		GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level);
		bool run();

	private:
		SDL_Renderer *renderer;
		Graphics graphics;
		Map map;
		std::vector<std::unique_ptr<Enemy>> activeEnemies;
		std::vector<std::unique_ptr<Projectil>> activeProjectils;
		Level level;
		bool running;
		float lastSpawnTime;
		float lastUpdateTime;
		int spawnedEnemyCount;
		size_t enemyTypeIndex;
		std::vector<Tower> towers;

		bool handleEvents();
		void update(float deltaTime);
		void render(float deltaTime);
		void spawnEnemies();
		void replaceSpacesWithTowers();
		void updateTowers(float currentTime);
		void checkProjectilCollisions();
	};
}

#endif