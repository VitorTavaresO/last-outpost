#ifndef __LAST_OUTPOST_LEVEL_H__
#define __LAST_OUTPOST_LEVEL_H__

#include <string>
#include <vector>
#include <last-outpost/enemy.h>

namespace Game
{
    class Level
    {
    private:
        std::string mapData;
        std::vector<Enemy> enemies;
        float enemyCount;

    public:
        Level(const std::string &mapData, const std::vector<Enemy> &enemies, float enemyCount)
            : mapData(mapData), enemies(enemies) {}

        const std::string &getMapData() const
        {
            return mapData;
        }
        const std::vector<Enemy> &getEnemies() const
        {
            return enemies;
        }
        float getEnemyCount() const
        {
            return enemyCount;
        }
    };
}

#endif