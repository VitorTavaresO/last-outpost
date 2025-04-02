#include <last-outpost/enemy.h>

namespace Game
{
    Enemy::Enemy(int life, int damage, float speed, const std::string &spell)
        : life(life), damage(damage), speed(speed), spell(spell), currentStep(0), lastMoveTime(0)
    {
    }

    void Enemy::update(const std::vector<std::pair<int, int>> &path)
    {
        if (path.empty() || currentStep >= path.size() - 1)
        {
            return;
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 moveInterval = static_cast<Uint32>(1000 / speed);

        float t = static_cast<float>(currentTime - lastMoveTime) / moveInterval;

        if (t >= 1.0f)
        {
            lastMoveTime = currentTime;
            ++currentStep;
            t = 0.0f;
        }

        auto [currentX, currentY] = path[currentStep];
        auto [nextX, nextY] = path[currentStep + 1];

        float interpolatedX = currentX + t * (nextX - currentX);
        float interpolatedY = currentY + t * (nextY - currentY);

        setPosition(interpolatedX, interpolatedY);
    }

    void Enemy::render(Graphics &graphics) const
    {
        graphics.drawRect(getPosition(), getSize(), {255, 0, 0, 255});
    }
}