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
		this->replaceSpacesWithTowers();
	}

	bool GameWorld::run()
	{
		this->lastUpdateTime = getTimeInSeconds();

		while (this->running)
		{
			float currentTime = getTimeInSeconds();
			float deltaTime = currentTime - this->lastUpdateTime;

			this->lastUpdateTime = currentTime;

			if (!this->handleEvents())
			{
				return false;
			}

			this->update(deltaTime);
			this->render(deltaTime);

			/*if (this->activeEnemies.empty() && this->spawnedEnemyCount >= this->level.getEnemyCount())
			{
				return true;
			}*/
		}

		return false;
	}

	bool GameWorld::handleEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				this->running = false;
				return false;
			}
		}
		return true;
	}

	void GameWorld::update(float deltaTime)
	{
		for (auto &enemy : this->activeEnemies)
		{
			enemy->update(deltaTime);
		}

		for (auto it = this->activeProjectils.begin(); it != this->activeProjectils.end();)
		{
			(*it)->update(deltaTime);

			if ((*it)->hasReachedTarget())
			{
				it = this->activeProjectils.erase(it);
			}
			else
			{
				++it;
			}
		}

		this->updateTowers(getTimeInSeconds());

		this->checkProjectilCollisions();

		this->spawnEnemies();
	}

	void GameWorld::render(float deltaTime)
	{
		SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
		SDL_RenderClear(this->renderer);

		this->map.render(this->graphics, deltaTime);

		for (const auto &enemy : this->activeEnemies)
		{
			enemy->render(this->graphics, deltaTime);
		}

		for (const auto &tower : this->towers)
		{
			tower.render(this->graphics, deltaTime);
		}

		for (const auto &projectil : this->activeProjectils)
		{
			projectil->render(this->graphics, deltaTime);
		}

		SDL_RenderPresent(this->renderer);
	}

	void GameWorld::spawnEnemies()
	{
		float currentTime = getTimeInSeconds();
		constexpr float spawnInterval = 2.0f;

		if (this->spawnedEnemyCount < this->level.getEnemyCount() &&
			currentTime - this->lastSpawnTime >= spawnInterval)
		{
			if (!this->level.getEnemyTypes().empty())
			{
				const Enemy &enemyTemplate = this->level.getEnemyTypes()[this->enemyTypeIndex];

				auto enemy = std::make_unique<Enemy>(
					enemyTemplate.getLife(),
					enemyTemplate.getDamage(),
					enemyTemplate.getSpeed(),
					enemyTemplate.getSpell(),

					map.extractPath());

				this->activeEnemies.push_back(std::move(enemy));
				++this->spawnedEnemyCount;
				this->lastSpawnTime = currentTime;

				this->enemyTypeIndex = (this->enemyTypeIndex + 1) % this->level.getEnemyTypes().size();
			}
		}
	}

	void GameWorld::updateTowers(float currentTime)
	{
		for (auto &tower : this->towers)
		{
			auto projectil = tower.findTargetAndTryFireAt(this->activeEnemies, currentTime);

			if (projectil)
			{
				this->activeProjectils.push_back(std::move(projectil));
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
					Projectil towerProjectil(50, 2.0f);
					Tower tower(5.0f, std::move(towerProjectil), {0, 0, 255, 255});
					tower.setPosition(col, row);

					auto towerSprite = std::make_unique<Sprite>();
					if (towerSprite->loadFromFile("assets/tower.png", renderer))
					{
						towerSprite->setScale(2.0f, 2.0f);
						tower.setSprite(std::move(towerSprite));
					}

					towers.push_back(std::move(tower));
				}
			}
		}
	}

	void GameWorld::checkProjectilCollisions()
	{
		for (auto projIt = this->activeProjectils.begin(); projIt != this->activeProjectils.end();)
		{
			bool hit = false;
			Enemy *targetEnemy = (*projIt)->getTargetEnemy();

			if (targetEnemy)
			{
				auto enemyIt = this->activeEnemies.begin();
				for (; enemyIt != this->activeEnemies.end(); ++enemyIt)
				{
					if ((*enemyIt).get() == targetEnemy)
					{
						break;
					}
				}

				if (enemyIt != this->activeEnemies.end())
				{
					if ((*projIt)->isColliding(**enemyIt))
					{
						int currentLife = (*enemyIt)->getLife();
						currentLife -= (*projIt)->getDamage();
						(*enemyIt)->setLife(currentLife);

						if (currentLife <= 0)
						{
							enemyIt = this->activeEnemies.erase(enemyIt);
						}

						hit = true;
					}
				}
				else
				{
					hit = true;
				}
			}

			if (hit)
			{
				projIt = this->activeProjectils.erase(projIt);
			}
			else
			{
				++projIt;
			}
		}
	}
}