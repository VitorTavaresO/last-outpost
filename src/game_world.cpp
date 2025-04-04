#include <utility>
#include <last-outpost/game_world.h>
#include <last-outpost/globals.h>

namespace Game
{
	GameWorld::GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level)
		: renderer(renderer),
		  graphics(screenWidth, screenHeight, TILES_X, TILES_Y, renderer),
		  map(TILES_X, TILES_Y, level.getMapData()),
		  level(std::move(level)),
		  running(true),
		  lastSpawnTime(0),
		  lastUpdateTime(SDL_GetTicks()),
		  spawnedEnemyCount(0)
	{
	}

	void GameWorld::run()
	{
		while (running)
		{
			handleEvents();
			update();
			render();
		}
	}

	void GameWorld::handleEvents()
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

	void GameWorld::update()
	{
		uint32_t currentTime = SDL_GetTicks();

		float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
		lastUpdateTime = currentTime;

		for (auto &enemy : activeEnemies)
		{
			enemy.update(deltaTime);
		}

		spawnEnemies();
	}

	void GameWorld::render()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		uint32_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;

		map.render(graphics, deltaTime);
		for (const auto &enemy : activeEnemies)
		{
			enemy.render(graphics, deltaTime);
		}

		SDL_RenderPresent(renderer);
	}

	void GameWorld::spawnEnemies()
	{
		uint32_t currentTime = SDL_GetTicks();
		constexpr uint32_t spawnInterval = 2000;

		static size_t enemyTypeIndex = 0;

		if (spawnedEnemyCount < level.getEnemyCount() && currentTime - lastSpawnTime >= spawnInterval)
		{
			if (!level.getEnemyTypes().empty())
			{
				const Enemy &enemy = level.getEnemyTypes()[enemyTypeIndex];

				activeEnemies.push_back(enemy);
				++spawnedEnemyCount;
				lastSpawnTime = currentTime;

				enemyTypeIndex = (enemyTypeIndex + 1) % level.getEnemyTypes().size();
			}
		}
	}
}