#ifndef __LAST_OUTPOST_ENEMY_H__
#define __LAST_OUTPOST_ENEMY_H__

#include <last-outpost/object.h>
#include <string>
#include <vector>
#include <utility>

namespace Game
{
    class Enemy : public Object
    {
    private:
        int life;
        int damage;
        float speed;
        std::string spell;

    public:
        Enemy(int life = 100, int damage = 10, float speed = 1.0f, const std::string &spell = "");

        void setLife(int life)
        {
            this->life = life;
        }

        int getLife() const
        {
            return life;
        }

        void setDamage(int damage)
        {
            this->damage = damage;
        }

        int getDamage() const
        {
            return damage;
        }

        void setSpeed(float speed)
        {
            this->speed = speed;
        }

        float getSpeed() const
        {
            return speed;
        }

        void setSpell(const std::string &spell)
        {
            this->spell = spell;
        }

        const std::string &getSpell() const
        {
            return spell;
        }

        void update() override;
        void render(Graphics &graphics) const override;
        bool followPath(const std::vector<std::pair<int, int>> &path);
    };
}

#endif