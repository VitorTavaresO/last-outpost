#include <last-outpost/enemy.h>
#include <last-outpost/types.h>
#include <utility>

namespace Game
{
	Enemy::Enemy(int life, int damage, float speed, const std::string &spell, std::vector<PathPoint> path)
		: life(life), damage(damage), speed(speed), spell(spell), path(std::move(path)), lastMoveTime(0), currentStep(0)
	{
		this->setType(ObjectType::Enemy);
		if (this->path.empty())
		{
			this->currentStep = 0;
			this->setPosition(0, 0);
		}
		else
		{
			this->currentStep = 0;
			this->setPosition(this->path[0].x, this->path[0].y);
		}
	}

	void Enemy::update(float deltaTime)
	{
		if (this->path.empty() || this->currentStep >= this->path.size() - 1)
		{
			return;
		}

		const auto currentPos = this->getPosition();
		const auto nextPos = Point(this->path[this->currentStep + 1].x, this->path[this->currentStep + 1].y);
		const auto direction = nextPos - currentPos;
		const float distance = direction.length();

		const float distanceTraveled = this->speed * deltaTime;

		const auto newPos = currentPos + Mylib::Math::with_length(direction, distanceTraveled);

		if (distanceTraveled >= distance)
		{
			this->currentStep++;
			if (this->currentStep >= this->path.size())
			{
				return;
			}
		}

		this->setPosition(newPos);
	}

	void Enemy::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawRect(this->getPosition(), {1, 1}, {255, 0, 0, 255});
	}
}