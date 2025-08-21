#include <last-outpost/tower.h>
#include <last-outpost/graphics.h>
#include <last-outpost/enemy.h>
#include <cmath>

namespace Game
{
	void Tower::update(float deltaTime)
	{
		if (this->animation)
		{
			this->animation->setPosition(this->getPosition().x, this->getPosition().y);
			this->animation->update(deltaTime);
		}

		if (this->currentState == TowerState::Attacking)
		{
			this->attackAnimationTimer += deltaTime;
			if (this->attackAnimationTimer >= this->attackAnimationDuration)
			{
				setState(TowerState::Idle);
				this->attackAnimationTimer = 0.0f;
			}
		}
	}
	void Tower::render(Graphics &graphics, float deltaTime) const
	{
		if (this->animation)
		{
			this->animation->render(graphics);
		}
		else
		{
			Object::render(graphics, deltaTime);
		}

		graphics.drawCircle(this->getPosition(), this->range, {163, 163, 163, 40});
	}

	void Tower::renderWithOverlay(Graphics &graphics, float deltaTime, const SDL_Color &overlayColor) const
	{
		if (this->animation)
		{
			this->animation->renderWithOverlay(graphics, overlayColor);
		}
		else
		{
			Object::render(graphics, deltaTime);
			graphics.drawRect(this->getPosition(), {1, 1}, overlayColor);
		}

		graphics.drawCircle(this->getPosition(), this->range, {163, 163, 163, 40});
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

		newProjectil->cloneVisualConfiguration(this->projectil, this->renderer);

		newProjectil->setTargetEnemy(enemy);

		setState(TowerState::Attacking);
		attackAnimationTimer = 0.0f;

		return newProjectil;
	}
	void Tower::setState(TowerState state)
	{
		if (this->currentState == state || !this->animation)
			return;

		this->currentState = state;

		switch (state)
		{
		case TowerState::Idle:
			this->animation->setFrame(0, 0);
			this->animation->pause();
			break;
		case TowerState::Attacking:
			this->animation->setFrame(0, 1);
			this->animation->pause();
			break;
		}
	}
}