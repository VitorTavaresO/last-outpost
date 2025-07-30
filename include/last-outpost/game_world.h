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
#include <last-outpost/audio.h>

namespace Game
{
	struct EnemyType
	{
		int life;
		int damage;
		float speed;
		std::string spell;
		std::string spriteAsset;
		int spriteWidth;
		int spriteHeight;
		int spriteCols;
		int spriteRows;
		float walkFrameTime;
		float idleFrameTime;
		int walkFrameStart;
		int walkFrameEnd;
		int idleFrameStart;
		int idleFrameEnd;
		float scale;
		int goldReward;
	};

	class GameWorld
	{
	public:
		GameWorld(SDL_Renderer *renderer, int screenWidth, int screenHeight, Level &&level, Audio *audioSystem = nullptr);
		bool run();

	private:
		SDL_Renderer *renderer;
		Graphics graphics;
		Map map;
		std::vector<std::unique_ptr<Enemy>> activeEnemies;
		std::vector<std::unique_ptr<Projectil>> activeProjectils;
		Level level;
		Audio *audioSystem;
		bool running;
		float lastSpawnTime;
		float lastUpdateTime;
		int spawnedEnemyCount;
		size_t enemyTypeIndex;
		std::vector<Tower> towers;
		std::vector<EnemyType> enemyTypes;

		int gold;

		bool tileSelected;
		int selectedRow;
		int selectedCol;

		bool towerSelected;
		int selectedTowerIndex;

		bool handleEvents();
		void update(float deltaTime);
		void render(float deltaTime);
		void spawnEnemies();
		void replaceSpacesWithTowers();
		void updateTowers(float currentTime);
		void checkProjectilCollisions();
		void initializeEnemyTypes();
		std::unique_ptr<Enemy> createEnemyFromType(const EnemyType &enemyType) const;

		void handleTileSelection(int mouseX, int mouseY);
		void handleTowerPlacement(int towerType);
		void placeTower(int row, int col, int towerType);
		bool isTileValidForTower(int row, int col) const;

		void handleTowerSelection(int mouseX, int mouseY);
		void deleteTower();
		void sellTower();
		int getTowerAtPosition(int row, int col) const;

		int getGold() const { return gold; }
		void addGold(int amount) { gold += amount; }
		bool spendGold(int amount);
		void renderUI();
		int getTowerCost(int towerType) const;

		void renderSimpleText(const std::string &text, int x, int y);
		void renderNumber(int number, int x, int y);
	};
}

#endif