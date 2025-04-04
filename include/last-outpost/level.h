#ifndef __LAST_OUTPOST_LEVEL_H__
#define __LAST_OUTPOST_LEVEL_H__

#include <string>
#include <vector>
#include <last-outpost/enemy.h>
#include <utility>

namespace Game
{
	class Level
	{
	private:
		std::string mapData;
		std::vector<Enemy> enemyTypes;
		float enemyCount;

	public:
		Level(const std::string &mapData, std::vector<Enemy> &&enemyTypes, float enemyCount)
			: mapData(mapData), enemyTypes(std::move(enemyTypes)), enemyCount(enemyCount)
		{
		}

		const std::string getMapData() const
		{
			return mapData;
		}

		const std::vector<Enemy> &getEnemyTypes() const
		{
			return enemyTypes;
		}

		float getEnemyCount() const
		{
			return enemyCount;
		}
	};
}

#endif