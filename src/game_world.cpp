#include <utility>
#include <memory>
#include <last-outpost/game_world.h>
#include <last-outpost/tower.h>
#include <last-outpost/globals.h>

namespace Game
{
	GameWorld::GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level)
		: renderer(renderer),
		  graphics(screenWidth, screenHeight, TILES_X, TILES_Y, renderer),
		  map(TILES_X, TILES_Y, level.getMapData()),
		  level(std::move(level)),
		  running(true),
		  lastSpawnTime(getTimeInSeconds()),
		  lastUpdateTime(getTimeInSeconds()),
		  spawnedEnemyCount(0),
		  enemyTypeIndex(0)
	{
		replaceSpacesWithTowers();
	}

	void GameWorld::run()
	{
		lastUpdateTime = getTimeInSeconds();

		while (running)
		{
			float currentTime = getTimeInSeconds();
			float deltaTime = currentTime - lastUpdateTime;

			lastUpdateTime = currentTime;

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
		for (auto &enemy : activeEnemies)
		{
			enemy->update(deltaTime);
		}

		for (auto it = activeProjectils.begin(); it != activeProjectils.end();)
		{
			(*it)->update(deltaTime);

			if ((*it)->hasReachedTarget())
			{
				it = activeProjectils.erase(it);
			}
			else
			{
				++it;
			}
		}

		updateTowers(getTimeInSeconds());

		checkProjectilCollisions();

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

		for (const auto &tower : towers)
		{
			tower.render(graphics, deltaTime);
		}

		for (const auto &projectil : activeProjectils)
		{
			projectil->render(graphics, deltaTime);
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

	void GameWorld::updateTowers(float currentTime)
	{
		for (auto &tower : towers)
		{
			auto projectil = tower.findTargetAndFire(activeEnemies, currentTime);

			if (projectil)
			{
				activeProjectils.push_back(std::move(projectil));
			}
		}
	}

	void GameWorld::replaceSpacesWithTowers()
	{

		for (int row = 0; row < map.getHeight(); ++row)
		{
			for (int col = 0; col < map.getWidth(); ++col)
			{
				Tile &tile = map(row, col);
				if (tile.object.getType() == ObjectType::Space)
				{
					Tower tower(10.0f, Projectil(1, 1.0f), {0, 0, 255, 255});
					tower.setPosition(col, row);
					towers.push_back(tower);
				}
			}
		}
	}

	void GameWorld::checkProjectilCollisions()
	{
		for (auto projIt = activeProjectils.begin(); projIt != activeProjectils.end();)
		{
			bool hit = false;

			Enemy *targetEnemy = (*projIt)->getTargetEnemy();

			if (targetEnemy)
			{
				auto enemyIt = std::find_if(activeEnemies.begin(), activeEnemies.end(),
											[targetEnemy](const std::unique_ptr<Enemy> &e)
											{ return e.get() == targetEnemy; });

				if (enemyIt != activeEnemies.end())
				{
					Vector projPos = (*projIt)->getPosition();
					Vector enemyPos = (*enemyIt)->getPosition();
					float distSq = (projPos - enemyPos).length_squared();

					if (distSq < 0.5f)
					{
						int currentLife = (*enemyIt)->getLife();
						currentLife -= (*projIt)->getDamage();
						(*enemyIt)->setLife(currentLife);

						if (currentLife <= 0)
						{
							enemyIt = activeEnemies.erase(enemyIt);
						}

						hit = true;
					}
				}
				else
				{
					hit = true;
				}
			}
			else
			{
				for (auto enemyIt = activeEnemies.begin(); enemyIt != activeEnemies.end();)
				{
					Vector projPos = (*projIt)->getPosition();
					Vector enemyPos = (*enemyIt)->getPosition();
					float distSq = (projPos - enemyPos).length_squared();

					if (distSq < 0.5f)
					{
						int currentLife = (*enemyIt)->getLife();
						currentLife -= (*projIt)->getDamage();
						(*enemyIt)->setLife(currentLife);

						if (currentLife <= 0)
						{
							enemyIt = activeEnemies.erase(enemyIt);
						}
						else
						{
							++enemyIt;
						}

						hit = true;
						break;
					}
					else
					{
						++enemyIt;
					}
				}
			}

			if (hit)
			{
				projIt = activeProjectils.erase(projIt);
			}
			else
			{
				++projIt;
			}
		}
	}
}