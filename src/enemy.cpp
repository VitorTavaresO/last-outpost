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
}