#include <last-outpost/enemy.h>
#include <last-outpost/types.h>

namespace Game
{
	Enemy::Enemy(int life, int damage, float speed, const std::string &spell, const std::vector<PathPoint> &path)
		: life(life), damage(damage), speed(speed), spell(spell), path(path), lastMoveTime(0), currentStep(0)
	{
	}

	void Enemy::update(float deltaTime)
	{
		if (path.empty() || currentStep >= path.size() - 1)
		{
			return;
		}

		const auto currentPos = getPosition();
		const auto nextPos = Point(path[currentStep + 1].x, path[currentStep + 1].y);
		const auto direction = nextPos - currentPos;
		const float distance = direction.length();

		const float distanceTraveled = speed * deltaTime;

		const auto newPos = currentPos + Mylib::Math::normalize(direction) * distanceTraveled;

		Point position(newPos.x, newPos.y);

		if (distanceTraveled >= distance)
		{
			currentStep++;
			if (currentStep >= path.size())
			{
				return;
			}
		}

		setPosition(position);
	}

	void Enemy::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(getPosition(), {1, 1}, {255, 0, 0, 255});
	}
}