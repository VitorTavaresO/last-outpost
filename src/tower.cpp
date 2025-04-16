#include <last-outpost/tower.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <cmath>

namespace Game
{
	void Tower::update(float deltaTime)
	{
	}

	void Tower::render(Graphics &graphics, float deltaTime) const
	{
		graphics.drawCircle(this->getPosition(), this->range, {0, 0, 255, 40});
		graphics.drawRect(this->getPosition(), this->getSize(), this->getColor());
	}

	bool Tower::isWithinRange(const Vector &targetPosition) const
	{
		float distanceSquared = (this->getPosition() - targetPosition).length_squared();
		return distanceSquared <= (this->range * this->range);
	}

	std::unique_ptr<Projectil> Tower::findTargetAndTryFireAt(const std::vector<std::unique_ptr<Enemy>> &enemies, float currentTime)
	{
		for (const auto &enemy : enemies)
		{
			auto projectil = this->tryFireAt(enemy.get(), currentTime);
			if (projectil)
			{
				return projectil;
			}
		}
		return nullptr;
	}

	std::unique_ptr<Projectil> Tower::tryFireAt(Enemy *enemy, float currentTime)
	{
		if (!enemy)
			return nullptr;

		if (currentTime - this->lastFireTime < this->fireRate)
			return nullptr;

		if (!this->isWithinRange(enemy->getPosition()))
			return nullptr;

		this->lastFireTime = currentTime;

		auto newProjectil = std::make_unique<Projectil>(
			this->projectil.getDamage(),
			this->projectil.getSpeed(),
			this->getPosition());

		newProjectil->setColor(this->projectil.getColor());
		newProjectil->setSize(this->projectil.getSize().x, this->projectil.getSize().y);

		newProjectil->setTargetEnemy(enemy);

		return newProjectil;
	}
}