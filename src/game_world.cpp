#include <utility>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <last-outpost/game_world.h>
#include <last-outpost/tower.h>
#include <last-outpost/globals.h>
#include <last-outpost/animation.h>

namespace Game
{
	GameWorld::GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level, Audio *audioSystem, UISystem *uiSystem)
		: renderer(renderer),
		  graphics(screenWidth, screenHeight, TILES_X, TILES_Y, renderer),
		  map(TILES_X, TILES_Y, level.getMapData(), renderer),
		  level(std::move(level)),
		  audioSystem(audioSystem),
		  uiSystem(uiSystem),
		  running(true),
		  lastSpawnTime(getTimeInSeconds()),
		  lastUpdateTime(getTimeInSeconds()),
		  spawnedEnemyCount(0),
		  enemyTypeIndex(0),
		  screenWidth(screenWidth),
		  screenHeight(screenHeight),
		  gameAreaWidth(screenWidth * 4 / 5),
		  tileSelected(false),
		  selectedRow(-1),
		  selectedCol(-1),
		  towerSelected(false),
		  selectedTowerIndex(-1),
		  gold(100),
		  playerLife(100)
	{
		this->initializeEnemyTypes();

		// Setup UI callbacks
		if (uiSystem)
		{
			uiSystem->setOnTowerSelected([this](TowerType towerType)
										 {
											 // Tower type selected for placement
										 });

			uiSystem->setOnTowerSell([this]()
									 { sellSelectedTower(); });

			uiSystem->setOnTowerUpgrade([this]()
										{ upgradeSelectedTower(); });

			uiSystem->setOnPlacementCancel([this]()
										   { clearSelection(); });
		}
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
			if (uiSystem)
			{
				uiSystem->handleEvent(event);
			}

			if (event.type == SDL_QUIT)
			{
				this->running = false;
				return false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (event.button.x < gameAreaWidth)
					{
						handleTileSelection(event.button.x, event.button.y);
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					if (event.button.x < gameAreaWidth)
					{
						handleTowerSelection(event.button.x, event.button.y);
					}
				}
			}
		}
		return true;
	}
	void GameWorld::update(float deltaTime)
	{
		auto enemyIt = this->activeEnemies.begin();
		while (enemyIt != this->activeEnemies.end())
		{
			(*enemyIt)->update(deltaTime);

			if ((*enemyIt)->getCurrentStep() >= (*enemyIt)->getPath().size())
			{
				int damage = (*enemyIt)->getDamage();
				this->takeDamage(damage);

				enemyIt = this->activeEnemies.erase(enemyIt);
			}
			else
			{
				++enemyIt;
			}
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

		SDL_Rect gameViewport = {0, 0, gameAreaWidth, screenHeight};
		SDL_RenderSetViewport(this->renderer, &gameViewport);

		this->map.render(this->graphics, deltaTime);

		if (tileSelected && selectedRow >= 0 && selectedCol >= 0)
		{
			Vector selectedPos(selectedCol, selectedRow);
			graphics.drawRect(selectedPos, {1, 1}, {255, 255, 0, 100});
		}

		for (const auto &enemy : this->activeEnemies)
		{
			enemy->render(this->graphics, deltaTime);
		}

		for (size_t i = 0; i < this->towers.size(); ++i)
		{
			if (towerSelected && selectedTowerIndex == static_cast<int>(i))
			{
				this->towers[i].renderWithOverlay(this->graphics, deltaTime, {255, 255, 0, 100});
			}
			else
			{
				this->towers[i].render(this->graphics, deltaTime);
			}
		}

		for (const auto &projectil : this->activeProjectils)
		{
			projectil->render(this->graphics, deltaTime);
		}

		SDL_RenderSetViewport(this->renderer, nullptr);

		if (uiSystem)
		{
			uiSystem->beginFrame();

			// Get selected tower info if any tower is selected
			SelectedTowerInfo *selectedInfo = nullptr;
			SelectedTowerInfo towerInfo;
			if (towerSelected && selectedTowerIndex >= 0 && selectedTowerIndex < static_cast<int>(towers.size()))
			{
				towerInfo = getSelectedTowerInfo();
				selectedInfo = &towerInfo;
			}

			uiSystem->renderUI(screenWidth, screenHeight, gold, playerLife, selectedInfo);
			uiSystem->endFrame();
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
		for (size_t i = 0; i < this->towers.size(); ++i)
		{
			auto projectil = towers[i].findTargetAndTryFireAt(this->activeEnemies, currentTime);

			if (projectil)
			{
				if (audioSystem)
				{
					float volumeVariation = 0.8f + (rand() % 40) / 100.0f;
					audioSystem->playSoundWithVolume(SoundType::TowerFire, volumeVariation);
				}
				this->activeProjectils.push_back(std::move(projectil));
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
						if (audioSystem)
						{
							audioSystem->playSound(SoundType::ProjectilHit);
						}

						int currentLife = (*enemyIt)->getLife();
						currentLife -= (*projIt)->getDamage();
						(*enemyIt)->setLife(currentLife);

						if (currentLife <= 0)
						{
							if (audioSystem)
							{
								audioSystem->playSound(SoundType::EnemyDeath);
							}

							this->addGold((*enemyIt)->getGoldReward());

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
		basicEnemy.damage = 3;
		basicEnemy.speed = 1.0f;
		basicEnemy.spell = "basic";
		basicEnemy.spriteAsset = "assets/sprites/enemies/base-enemy.png";
		basicEnemy.spriteWidth = 460;
		basicEnemy.spriteHeight = 460;
		basicEnemy.spriteCols = 5;
		basicEnemy.spriteRows = 2;
		basicEnemy.walkFrameTime = 0.2f;
		basicEnemy.idleFrameTime = 0.5f;
		basicEnemy.walkFrameStart = 0;
		basicEnemy.walkFrameEnd = 3;
		basicEnemy.idleFrameStart = 0;
		basicEnemy.idleFrameEnd = 1;
		basicEnemy.scale = 0.1f;
		basicEnemy.goldReward = 10;

		this->enemyTypes.push_back(basicEnemy);

		EnemyType fastEnemy;
		fastEnemy.life = 60;
		fastEnemy.damage = 5;
		fastEnemy.speed = 2.0f;
		fastEnemy.spell = "speed";
		fastEnemy.spriteAsset = "assets/sprites/enemies/fast-enemy.png";
		fastEnemy.spriteWidth = 580;
		fastEnemy.spriteHeight = 580;
		fastEnemy.spriteCols = 4;
		fastEnemy.spriteRows = 2;
		fastEnemy.walkFrameTime = 0.1f;
		fastEnemy.idleFrameTime = 0.3f;
		fastEnemy.walkFrameStart = 0;
		fastEnemy.walkFrameEnd = 3;
		fastEnemy.idleFrameStart = 0;
		fastEnemy.idleFrameEnd = 1;
		fastEnemy.scale = 0.08f;
		fastEnemy.goldReward = 5;

		this->enemyTypes.push_back(fastEnemy);

		EnemyType strongEnemy;
		strongEnemy.life = 200;
		strongEnemy.damage = 10;
		strongEnemy.speed = 0.5f;
		strongEnemy.spell = "tank";
		strongEnemy.spriteAsset = "assets/sprites/enemies/giant-enemy.png";
		strongEnemy.spriteWidth = 530;
		strongEnemy.spriteHeight = 530;
		strongEnemy.spriteCols = 5;
		strongEnemy.spriteRows = 2;
		strongEnemy.walkFrameTime = 0.4f;
		strongEnemy.idleFrameTime = 0.8f;
		strongEnemy.walkFrameStart = 0;
		strongEnemy.walkFrameEnd = 3;
		strongEnemy.idleFrameStart = 0;
		strongEnemy.idleFrameEnd = 1;
		strongEnemy.scale = 0.12f;
		strongEnemy.goldReward = 50;

		this->enemyTypes.push_back(strongEnemy);
	}

	std::unique_ptr<Enemy> GameWorld::createEnemyFromType(const EnemyType &enemyType) const
	{
		auto enemy = std::make_unique<Enemy>(
			enemyType.life,
			enemyType.damage,
			enemyType.speed,
			enemyType.spell,
			map.extractPath(),
			enemyType.goldReward);

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
			// Check if there's a tower at this position first
			int towerIndex = getTowerAtPosition(row, col);
			if (towerIndex >= 0)
			{
				// Select the tower
				towerSelected = true;
				selectedTowerIndex = towerIndex;

				tileSelected = false;
				selectedRow = -1;
				selectedCol = -1;
				return;
			}

			// If no tower and tile is valid for placement
			if (isTileValidForTower(row, col))
			{
				tileSelected = true;
				selectedRow = row;
				selectedCol = col;

				towerSelected = false;
				selectedTowerIndex = -1;

				if (uiSystem && uiSystem->isTowerSelected())
				{
					TowerType selectedTowerType = uiSystem->getSelectedTower();
					int towerTypeInt = static_cast<int>(selectedTowerType) + 1;
					handleTowerPlacement(towerTypeInt);
					uiSystem->clearTowerSelection();
				}
			}
		}
	}

	void GameWorld::handleTowerPlacement(int towerType)
	{
		if (tileSelected && selectedRow >= 0 && selectedCol >= 0)
		{
			int cost = getTowerCost(towerType);
			if (spendGold(cost))
			{
				placeTower(selectedRow, selectedCol, towerType);

				tileSelected = false;
				selectedRow = -1;
				selectedCol = -1;

				towerSelected = false;
				selectedTowerIndex = -1;
			}
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
			tower = Tower(5.0f, std::move(magicProjectil), {0, 0, 255, 255}, renderer);
			tower.setFireRate(1.0f);
			towerName = "Magic Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/towers/magic-tower.png", renderer,
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

			tower.setProjectileAnimation("assets/sprites/projectiles/projectile.png",
										 256, 256,
										 4, 1,
										 0.1f,
										 0, 3,
										 0.1f);
		}
		break;

		case 2:
		{
			Projectil fireProjectil(60, 2.5f);
			tower = Tower(3.5f, std::move(fireProjectil), {255, 165, 0, 255}, renderer);
			tower.setFireRate(2.0f);
			towerName = "Fire Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/towers/fire-tower.png", renderer,
															  360, 360, 4, 3);
			if (towerAnimation->isValid())
			{
				towerAnimation->setFrameTime(0.2f);
				towerAnimation->setScale(0.25f, 0.25f);
				towerAnimation->setPosition(col, row);
				towerAnimation->setFrame(0, 0);
				towerAnimation->pause();

				tower.setAnimation(std::move(towerAnimation));
				tower.setState(TowerState::Idle);
			}

			tower.setProjectileAnimation("assets/sprites/projectiles/fireball.png",
										 256, 256,
										 4, 2,
										 0.15f,
										 4, 7,
										 0.3f);
		}
		break;

		case 3:
		{
			Projectil leafProjectil(30, 2.0f);
			tower = Tower(4.0f, std::move(leafProjectil), {0, 255, 0, 255}, renderer);
			tower.setFireRate(1.5f);
			towerName = "Leaf Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/towers/leaf-tower.png", renderer,
															  360, 360, 4, 3);
			if (towerAnimation->isValid())
			{
				towerAnimation->setFrameTime(0.2f);
				towerAnimation->setScale(0.25f, 0.25f);
				towerAnimation->setPosition(col, row);
				towerAnimation->setFrame(0, 0);
				towerAnimation->pause();

				tower.setAnimation(std::move(towerAnimation));
				tower.setState(TowerState::Idle);
			}

			tower.setProjectileAnimation("assets/sprites/projectiles/projectile.png",
										 256, 256,
										 4, 1,
										 0.1f,
										 0, 3,
										 0.2f);
		}
		break;

		case 4:
		{
			Projectil thunderProjectil(40, 4.0f);
			tower = Tower(6.0f, std::move(thunderProjectil), {255, 255, 0, 255}, renderer);
			tower.setFireRate(3.0f);
			towerName = "Thunder Tower";

			auto towerAnimation = std::make_unique<Animation>("assets/sprites/towers/thunder-tower.png", renderer,
															  360, 360, 4, 3);
			if (towerAnimation->isValid())
			{
				towerAnimation->setFrameTime(0.2f);
				towerAnimation->setScale(0.25f, 0.25f);
				towerAnimation->setPosition(col, row);
				towerAnimation->setFrame(0, 0);
				towerAnimation->pause();

				tower.setAnimation(std::move(towerAnimation));
				tower.setState(TowerState::Idle);
			}

			tower.setProjectileAnimation("assets/sprites/projectiles/projectile.png",
										 256, 256,
										 4, 1,
										 0.1f,
										 0, 3,
										 0.05f);
		}
		break;

		default:
			return;
		}

		tower.setPosition(col, row);
		towers.push_back(std::move(tower));

		if (audioSystem)
		{
			audioSystem->playSound(SoundType::TowerPlace);
		}
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

		for (size_t i = 0; i < towers.size(); ++i)
		{
			Vector towerPos = towers[i].getPosition();

			int towerPixelX = static_cast<int>(towerPos.x * tileWidth);
			int towerPixelY = static_cast<int>(towerPos.y * tileHeight);

			int expandedWidth = static_cast<int>(tileWidth * 1.5f);
			int expandedHeight = static_cast<int>(tileHeight * 1.5f);

			if (mouseX >= towerPixelX - (expandedWidth - tileWidth) / 2 &&
				mouseX <= towerPixelX + expandedWidth - (expandedWidth - tileWidth) / 2 &&
				mouseY >= towerPixelY - (expandedHeight - tileHeight) / 2 &&
				mouseY <= towerPixelY + expandedHeight - (expandedHeight - tileHeight) / 2)
			{
				towerSelected = true;
				selectedTowerIndex = static_cast<int>(i);

				tileSelected = false;
				selectedRow = -1;
				selectedCol = -1;

				return;
			}
		}

		// If no tower was clicked, clear selection
		clearSelection();
	}

	void GameWorld::sellSelectedTower()
	{
		if (towerSelected && selectedTowerIndex >= 0 && selectedTowerIndex < static_cast<int>(towers.size()))
		{
			const Tower &tower = towers[selectedTowerIndex];

			int projectilDamage = tower.getProjectil().getDamage();
			int towerType = 1;

			if (projectilDamage == 50)
				towerType = 1; // Magic Tower
			else if (projectilDamage == 60)
				towerType = 2; // Fire Tower
			else if (projectilDamage == 30)
				towerType = 3; // Leaf Tower
			else if (projectilDamage == 40)
				towerType = 4; // Thunder Tower

			int originalCost = getTowerCost(towerType);
			int sellValue = originalCost / 2;

			addGold(sellValue);

			towers.erase(towers.begin() + selectedTowerIndex);

			clearSelection();

			if (audioSystem)
			{
				audioSystem->playSound(SoundType::TowerPlace); // Use same sound for sell
			}
		}
	}

	void GameWorld::upgradeSelectedTower()
	{
		if (towerSelected && selectedTowerIndex >= 0 && selectedTowerIndex < static_cast<int>(towers.size()))
		{
			// For now, just show that upgrade was attempted
			// This can be expanded later with actual upgrade logic
			std::cout << "Tower upgrade requested!" << std::endl;
		}
	}

	void GameWorld::clearSelection()
	{
		tileSelected = false;
		selectedRow = -1;
		selectedCol = -1;
		towerSelected = false;
		selectedTowerIndex = -1;

		if (uiSystem)
		{
			uiSystem->clearTowerSelection();
		}
	}

	SelectedTowerInfo GameWorld::getSelectedTowerInfo() const
	{
		SelectedTowerInfo info;

		if (towerSelected && selectedTowerIndex >= 0 && selectedTowerIndex < static_cast<int>(towers.size()))
		{
			const Tower &tower = towers[selectedTowerIndex];

			info.index = selectedTowerIndex;
			info.position = tower.getPosition();
			info.damage = tower.getProjectil().getDamage();
			info.range = tower.getRange();
			info.fireRate = tower.getFireRate();

			// Determine tower type based on damage (this is a simple way to identify type)
			if (info.damage == 50)
				info.type = TowerType::Magic;
			else if (info.damage == 60)
				info.type = TowerType::Fire;
			else if (info.damage == 30)
				info.type = TowerType::Leaf;
			else if (info.damage == 40)
				info.type = TowerType::Thunder;
			else
				info.type = TowerType::Magic; // Default

			int towerTypeInt = static_cast<int>(info.type) + 1;
			int originalCost = getTowerCost(towerTypeInt);
			info.sellValue = originalCost / 2;
			info.upgradeValue = originalCost; // Same cost as original for upgrade
		}

		return info;
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
		return -1;
	}

	bool GameWorld::spendGold(int amount)
	{
		if (gold >= amount)
		{
			gold -= amount;
			return true;
		}
		return false;
	}

	int GameWorld::getTowerCost(int towerType) const
	{
		switch (towerType)
		{
		case 1: // Magic Tower
			return 50;
		case 2: // Fire Tower
			return 75;
		case 3: // Leaf Tower
			return 60;
		case 4: // Thunder Tower
			return 80;
		default:
			return 0;
		}
	}

	void GameWorld::takeDamage(int damage)
	{
		playerLife -= damage;
		if (playerLife < 0)
		{
			playerLife = 0;
		}

		if (audioSystem)
		{
			audioSystem->playSound(SoundType::GameOver);
		}
	}
}