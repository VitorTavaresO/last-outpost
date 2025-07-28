#include <utility>
#include <memory>
#include <iostream>
#include <last-outpost/game_world.h>
#include <last-outpost/tower.h>
#include <last-outpost/globals.h>
#include <last-outpost/animation.h>

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
		  enemyTypeIndex(0),
		  tileSelected(false),
		  selectedRow(-1),
		  selectedCol(-1),
		  towerSelected(false),
		  selectedTowerIndex(-1)
	{
		this->initializeEnemyTypes();
		//  this->replaceSpacesWithTowers();
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
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					handleTileSelection(event.button.x, event.button.y);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					handleTowerSelection(event.button.x, event.button.y);
				}
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_1)
				{
					handleTowerPlacement(1); // Magic Tower
				}
				else if (event.key.keysym.sym == SDLK_2)
				{
					handleTowerPlacement(2); // Canon Tower
				}
				else if (event.key.keysym.sym == SDLK_DELETE)
				{
					deleteTower();
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					// Cancelar seleção
					tileSelected = false;
					selectedRow = -1;
					selectedCol = -1;
					towerSelected = false;
					selectedTowerIndex = -1;
				}
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

		for (auto &tower : this->towers)
		{
			tower.update(deltaTime);
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

		// Destacar tile selecionado
		if (tileSelected && selectedRow >= 0 && selectedCol >= 0)
		{
			Vector selectedPos(selectedCol, selectedRow);
			graphics.drawRect(selectedPos, {1, 1}, {255, 255, 0, 100}); // Amarelo transparente
		}

		for (const auto &enemy : this->activeEnemies)
		{
			enemy->render(this->graphics, deltaTime);
		}

		for (size_t i = 0; i < this->towers.size(); ++i)
		{
			this->towers[i].render(this->graphics, deltaTime);

			if (towerSelected && selectedTowerIndex == static_cast<int>(i))
			{
				Vector towerPos = this->towers[i].getPosition();
				graphics.drawRect(towerPos, {1, 1}, {255, 0, 0, 150});
			}
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
			if (!this->enemyTypes.empty())
			{
				const EnemyType &enemyType = this->enemyTypes[this->enemyTypeIndex];
				auto enemy = this->createEnemyFromType(enemyType);

				if (enemy)
				{
					this->activeEnemies.push_back(std::move(enemy));
					++this->spawnedEnemyCount;
					this->lastSpawnTime = currentTime;

					this->enemyTypeIndex = (this->enemyTypeIndex + 1) % this->enemyTypes.size();
				}
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

					auto towerAnimation = std::make_unique<Animation>("assets/sprites/magic-tower.png", renderer,
																	  400, 467, 4, 3);
					if (towerAnimation->isValid())
					{
						towerAnimation->setFrameTime(0.2f);
						towerAnimation->setScale(0.2f, 0.2f);
						towerAnimation->setPosition(col, row);

						towerAnimation->setFrame(0, 0);
						towerAnimation->pause();

						tower.setAnimation(std::move(towerAnimation));
						tower.setState(TowerState::Idle);
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

	void GameWorld::initializeEnemyTypes()
	{
		EnemyType basicEnemy;
		basicEnemy.life = 100;
		basicEnemy.damage = 10;
		basicEnemy.speed = 1.0f;
		basicEnemy.spell = "basic";
		basicEnemy.spriteAsset = "assets/sprites/base-enemy.png";
		basicEnemy.spriteWidth = 307;
		basicEnemy.spriteHeight = 512;
		basicEnemy.spriteCols = 5;
		basicEnemy.spriteRows = 2;
		basicEnemy.walkFrameTime = 0.2f;
		basicEnemy.idleFrameTime = 0.5f;
		basicEnemy.walkFrameStart = 0;
		basicEnemy.walkFrameEnd = 3;
		basicEnemy.idleFrameStart = 0;
		basicEnemy.idleFrameEnd = 1;
		basicEnemy.scale = 0.1f;

		this->enemyTypes.push_back(basicEnemy);

		EnemyType fastEnemy;
		fastEnemy.life = 60;
		fastEnemy.damage = 8;
		fastEnemy.speed = 2.0f;
		fastEnemy.spell = "speed";
		fastEnemy.spriteAsset = "assets/sprites/base-enemy.png";
		fastEnemy.spriteWidth = 307;
		fastEnemy.spriteHeight = 512;
		fastEnemy.spriteCols = 5;
		fastEnemy.spriteRows = 2;
		fastEnemy.walkFrameTime = 0.1f;
		fastEnemy.idleFrameTime = 0.3f;
		fastEnemy.walkFrameStart = 0;
		fastEnemy.walkFrameEnd = 3;
		fastEnemy.idleFrameStart = 0;
		fastEnemy.idleFrameEnd = 1;
		fastEnemy.scale = 0.08f;

		this->enemyTypes.push_back(fastEnemy);

		EnemyType strongEnemy;
		strongEnemy.life = 200;
		strongEnemy.damage = 20;
		strongEnemy.speed = 0.5f;
		strongEnemy.spell = "tank";
		strongEnemy.spriteAsset = "assets/sprites/base-enemy.png";
		strongEnemy.spriteWidth = 307;
		strongEnemy.spriteHeight = 512;
		strongEnemy.spriteCols = 5;
		strongEnemy.spriteRows = 2;
		strongEnemy.walkFrameTime = 0.4f;
		strongEnemy.idleFrameTime = 0.8f;
		strongEnemy.walkFrameStart = 0;
		strongEnemy.walkFrameEnd = 3;
		strongEnemy.idleFrameStart = 0;
		strongEnemy.idleFrameEnd = 1;
		strongEnemy.scale = 0.12f;

		this->enemyTypes.push_back(strongEnemy);
	}

	std::unique_ptr<Enemy> GameWorld::createEnemyFromType(const EnemyType &enemyType) const
	{
		auto enemy = std::make_unique<Enemy>(
			enemyType.life,
			enemyType.damage,
			enemyType.speed,
			enemyType.spell,
			map.extractPath());

		if (!enemy->loadAnimations(renderer,
								   enemyType.spriteAsset,
								   enemyType.spriteWidth,
								   enemyType.spriteHeight,
								   enemyType.spriteCols,
								   enemyType.spriteRows,
								   enemyType.walkFrameTime,
								   enemyType.idleFrameTime,
								   enemyType.walkFrameStart,
								   enemyType.walkFrameEnd,
								   enemyType.idleFrameStart,
								   enemyType.idleFrameEnd,
								   enemyType.scale))
		{
			return nullptr;
		}

		return enemy;
	}

	void GameWorld::handleTileSelection(int mouseX, int mouseY)
	{
		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();

		int col = mouseX / tileWidth;
		int row = mouseY / tileHeight;

		if (row >= 0 && row < map.getHeight() && col >= 0 && col < map.getWidth())
		{
			if (isTileValidForTower(row, col))
			{
				tileSelected = true;
				selectedRow = row;
				selectedCol = col;

				// Desmarcar torre selecionada
				towerSelected = false;
				selectedTowerIndex = -1;
			}
		}
	}

	void GameWorld::handleTowerPlacement(int towerType)
	{
		if (tileSelected && selectedRow >= 0 && selectedCol >= 0)
		{
			placeTower(selectedRow, selectedCol, towerType);

			tileSelected = false;
			selectedRow = -1;
			selectedCol = -1;

			// Desmarcar torre selecionada
			towerSelected = false;
			selectedTowerIndex = -1;
		}
	}

	void GameWorld::placeTower(int row, int col, int towerType)
	{

		Tower tower;
		std::string towerName;

		switch (towerType)
		{
		case 1:
		{
			Projectil magicProjectil(50, 3.0f);
			tower = Tower(5.0f, std::move(magicProjectil), {0, 0, 255, 255});
			tower.setFireRate(1.0f);
			towerName = "Magic Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/magic-tower.png", renderer,
															  400, 467, 4, 3);
			if (towerAnimation->isValid())
			{
				towerAnimation->setFrameTime(0.2f);
				towerAnimation->setScale(0.2f, 0.2f);
				towerAnimation->setPosition(col, row);
				towerAnimation->setFrame(0, 0);
				towerAnimation->pause();

				tower.setAnimation(std::move(towerAnimation));
				tower.setState(TowerState::Idle);
			}
		}
		break;

		case 2:
		{
			Projectil canonProjectil(80, 2.0f);
			tower = Tower(4.0f, std::move(canonProjectil), {255, 0, 0, 255});
			tower.setFireRate(1.5f);
			towerName = "Canon Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/magic-tower.png", renderer,
															  400, 467, 4, 3);
			if (towerAnimation->isValid())
			{
				towerAnimation->setFrameTime(0.3f);
				towerAnimation->setScale(0.25f, 0.25f);
				towerAnimation->setPosition(col, row);
				towerAnimation->setFrame(0, 0);
				towerAnimation->pause();

				tower.setAnimation(std::move(towerAnimation));
				tower.setState(TowerState::Idle);
			}
		}
		break;

		default:
			return;
		}

		tower.setPosition(col, row);
		towers.push_back(std::move(tower));
	}

	bool GameWorld::isTileValidForTower(int row, int col) const
	{
		if (row < 0 || row >= map.getHeight() || col < 0 || col >= map.getWidth())
			return false;

		const Tile &tile = map(row, col);

		if (tile.object.getType() != ObjectType::Space)
			return false;

		for (const auto &tower : towers)
		{
			Vector towerPos = tower.getPosition();
			if (static_cast<int>(towerPos.x) == col && static_cast<int>(towerPos.y) == row)
			{
				return false;
			}
		}

		return true;
	}

	void GameWorld::handleTowerSelection(int mouseX, int mouseY)
	{
		int tileWidth = graphics.getTileWidth();
		int tileHeight = graphics.getTileHeight();

		int col = mouseX / tileWidth;
		int row = mouseY / tileHeight;

		if (row >= 0 && row < map.getHeight() && col >= 0 && col < map.getWidth())
		{
			int towerIndex = getTowerAtPosition(row, col);
			if (towerIndex >= 0)
			{
				towerSelected = true;
				selectedTowerIndex = towerIndex;

				// Desmarcar tile selecionado
				tileSelected = false;
				selectedRow = -1;
				selectedCol = -1;

				std::cout << "Torre selecionada na posição (" << row << ", " << col << ") - Pressione DELETE para remover" << std::endl;
			}
		}
	}

	void GameWorld::deleteTower()
	{
		if (towerSelected && selectedTowerIndex >= 0 && selectedTowerIndex < static_cast<int>(towers.size()))
		{
			Vector towerPos = towers[selectedTowerIndex].getPosition();
			std::cout << "Torre removida da posição (" << static_cast<int>(towerPos.y) << ", " << static_cast<int>(towerPos.x) << ")" << std::endl;

			towers.erase(towers.begin() + selectedTowerIndex);

			// Limpar seleção
			towerSelected = false;
			selectedTowerIndex = -1;
		}
		else
		{
			std::cout << "Nenhuma torre selecionada para deletar!" << std::endl;
		}
	}

	int GameWorld::getTowerAtPosition(int row, int col) const
	{
		for (size_t i = 0; i < towers.size(); ++i)
		{
			Vector towerPos = towers[i].getPosition();
			if (static_cast<int>(towerPos.x) == col && static_cast<int>(towerPos.y) == row)
			{
				return static_cast<int>(i);
			}
		}
		return -1; // Nenhuma torre encontrada
	}
}