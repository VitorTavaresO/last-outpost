#include <last-outpost/game_world.h>

namespace Game
{
	constexpr int TILES_X = 32;
	constexpr int TILES_Y = 18;

	GameWorld::GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, const Level &level)
		: renderer(renderer),
		  graphics(screenWidth, screenHeight, TILES_X, TILES_Y, renderer),
		  map(TILES_X, TILES_Y, level.getMapData()),
		  level(level),
		  running(true),
		  lastSpawnTime(0),
		  lastUpdateTime(SDL_GetTicks()), // Inicializa lastUpdateTime aqui
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

		auto path = map.extractPath();
		for (auto &enemy : activeEnemies)
		{
			enemy.update(path, deltaTime);
		}

		spawnEnemies();
	}

	void GameWorld::render()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		map.render(graphics);
		for (const auto &enemy : activeEnemies)
		{
			enemy.render(graphics);
		}

		SDL_RenderPresent(renderer);
	}

	void GameWorld::spawnEnemies()
	{
		uint32_t currentTime = SDL_GetTicks();
		const uint32_t spawnInterval = 2000;

		static size_t enemyTypeIndex = 0;

		if (spawnedEnemyCount < level.getEnemyCount() && currentTime - lastSpawnTime >= spawnInterval)
		{
			if (!level.getEnemyTypes().empty())
			{
				Enemy enemy = level.getEnemyTypes()[enemyTypeIndex];

				auto path = map.extractPath();
				if (!path.empty())
				{
					enemy.setPosition(path[0].first, path[0].second);
				}

				activeEnemies.push_back(enemy);
				++spawnedEnemyCount;
				lastSpawnTime = currentTime;

				enemyTypeIndex = (enemyTypeIndex + 1) % level.getEnemyTypes().size();
			}
		}
	}
}