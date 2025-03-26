#include <last-outpost/enemy.h>

namespace Game
{
    Enemy::Enemy(int life, int damage, float speed, const std::string &spell)
        : life(life), damage(damage), speed(speed), spell(spell)
    {
    }

    void Enemy::update()
    {
        Vector pos = getPosition();
        pos.x += speed;
        setPosition(pos.x, pos.y);
    }

    void Enemy::render(Graphics &graphics) const
    {
        graphics.drawRect(getPosition(), getSize(), {255, 0, 0, 255});
    }

    bool Enemy::followPath(const std::vector<std::pair<int, int>> &path)
    {
        static size_t currentStep = 0;
        static Uint32 lastMoveTime = 0;

        if (path.empty())
        {
            return false;
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 moveInterval = static_cast<Uint32>(2000 / speed);

        if (currentStep < path.size())
        {
            if (currentTime - lastMoveTime >= moveInterval)
            {
                setPosition(path[currentStep].first, path[currentStep].second);
                ++currentStep;
                lastMoveTime = currentTime;
            }
            return true;
        }

        return false;
    }
}