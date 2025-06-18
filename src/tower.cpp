#include <last-outpost/tower.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <cmath>

namespace Game
{
	void Tower::update(float deltaTime)
	{
		if (animation)
		{
			animation->setPosition(this->getPosition().x, this->getPosition().y);
			animation->update(deltaTime);
		}

		if (currentState == TowerState::Attacking)
		{
			attackAnimationTimer += deltaTime;
			if (attackAnimationTimer >= attackAnimationDuration)
			{
				setState(TowerState::Idle);
				attackAnimationTimer = 0.0f;
			}
		}
	}
	void Tower::render(Graphics &graphics, float deltaTime) const
	{
		if (animation)
		{
			animation->render(graphics);
		}
		else
		{
			Object::render(graphics, deltaTime);
		}

		graphics.drawCircle(this->getPosition(), this->range, {0, 0, 255, 40});
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

		setState(TowerState::Attacking);
		attackAnimationTimer = 0.0f;

		return newProjectil;
	}
	void Tower::setState(TowerState state)
	{
		if (currentState == state || !animation)
			return;

		currentState = state;

		switch (state)
		{
		case TowerState::Idle:
			animation->setFrame(0, 0);
			animation->pause();
			break;
		case TowerState::Attacking:
			animation->setFrame(0, 1);
			animation->pause();
			break;
		}
	}
}