#include <last-outpost/enemy.h>

namespace Game
{
    Enemy::Enemy(int life, int damage, float speed, const std::string &spell)
        : life(life), damage(damage), speed(speed), spell(spell), currentStep(0), lastMoveTime(0)
    {
    }

    void Enemy::update(const std::vector<std::pair<int, int>> &path)
    {
        if (path.empty() || currentStep >= path.size())
        {
            return;
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 moveInterval = static_cast<Uint32>(1000 / speed);

        if (currentTime - lastMoveTime >= moveInterval)
        {
            setPosition(path[currentStep].first, path[currentStep].second);
            ++currentStep;
            lastMoveTime = currentTime;
        }
    }

    void Enemy::render(Graphics &graphics) const
    {
        graphics.drawRect(getPosition(), getSize(), {255, 0, 0, 255});
    }
}