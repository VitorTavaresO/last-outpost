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

		const auto currentPos = getPosition();
		const auto nextPos = Point(path[currentStep + 1].first, path[currentStep + 1].second);
		const auto direction = nextPos - currentPos;
		const float distance = direction.length();

		const float distanceTraveled = speed * deltaTime;

		const auto newPos = currentPos + Mylib::Math::normalize(direction) * distanceTraveled;

		if (distanceTraveled >= distance)
		{
			currentStep++;
			if (currentStep >= path.size())
			{
				return;
			}
		}

		setPosition(newPos.x, newPos.y);
	}

	void Enemy::render(Graphics &graphics) const
	{
		graphics.drawRect(getPosition(), {1, 1}, {255, 0, 0, 255});
	}
}