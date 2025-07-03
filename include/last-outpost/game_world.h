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
	};

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
		std::vector<EnemyType> enemyTypes;

		bool tileSelected;
		int selectedRow;
		int selectedCol;

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
	};
}

#endif