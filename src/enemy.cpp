#include <last-outpost/enemy.h>

namespace Game
{
    Enemy::Enemy(int life, int damage, float speed, const std::string &spell)
        : life(life), damage(damage), speed(speed), spell(spell), currentStep(0), lastMoveTime(0)
    {
    }

    void Enemy::update(const std::vector<std::pair<int, int>> &path, float deltaTime)
    {
        if (path.empty() || currentStep >= path.size() - 1)
        {
            return;
        }

        auto [currentX, currentY] = path[currentStep];
        auto [nextX, nextY] = path[currentStep + 1];

        float directionX = nextX - currentX;
        float directionY = nextY - currentY;

        float distance = std::sqrt(directionX * directionX + directionY * directionY);

        if (distance > 0.0f)
        {
            directionX /= distance;
            directionY /= distance;
        }

        float distanceTraveled = speed * deltaTime;

        float newX = getPosition().x + directionX * distanceTraveled;
        float newY = getPosition().y + directionY * distanceTraveled;

        float remainingDistance = std::sqrt((nextX - newX) * (nextX - newX) + (nextY - newY) * (nextY - newY));
        if (remainingDistance < distanceTraveled)
        {
            ++currentStep;
            setPosition(nextX, nextY);
        }
        else
        {
            setPosition(newX, newY);
        }
    }

    void Enemy::render(Graphics &graphics) const
    {
        graphics.drawRect(getPosition(), {1, 1}, {255, 0, 0, 255});
    }
}