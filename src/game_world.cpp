#include <utility>
#include <memory>
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
		  lastUpdateTime(getTimeInSeconds()),
		  spawnedEnemyCount(0),
		  enemyTypeIndex(0)
	{
	}

	void GameWorld::run()
	{
		while (running)
		{
			float currentTime = SDL_GetTicks();
			float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;

			handleEvents();
			update(deltaTime);
			render(deltaTime);
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

	void GameWorld::update(float deltaTime)
	{
		float currentTime = SDL_GetTicks();
		lastUpdateTime = currentTime;

		for (auto &enemy : activeEnemies)
		{
			enemy->update(deltaTime);
		}

		spawnEnemies();
	}

	void GameWorld::render(float deltaTime)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		map.render(graphics, deltaTime);
		for (const auto &enemy : activeEnemies)
		{
			enemy->render(graphics, deltaTime);
		}

		SDL_RenderPresent(renderer);
	}

	void GameWorld::spawnEnemies()
	{
		float currentTime = getTimeInSeconds();
		constexpr float spawnInterval = 2.0f;

		if (spawnedEnemyCount < level.getEnemyCount() && currentTime - lastSpawnTime >= spawnInterval)
		{
			if (!level.getEnemyTypes().empty())
			{
				auto enemy = std::make_unique<Enemy>(level.getEnemyTypes()[enemyTypeIndex]);

				activeEnemies.push_back(std::move(enemy));
				++spawnedEnemyCount;
				lastSpawnTime = currentTime;

				enemyTypeIndex = (enemyTypeIndex + 1) % level.getEnemyTypes().size();
			}
		}
	}
}