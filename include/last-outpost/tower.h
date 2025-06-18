#ifndef LAST_OUTPOST_TOWER_H
#define LAST_OUTPOST_TOWER_H

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/types.h>
#include <last-outpost/projectil.h>
#include <last-outpost/animation.h>
#include <memory>
#include <vector>

namespace Game
{
	class Enemy;

	enum class TowerState
	{
		Idle,
		Attacking
	};

	class Tower : public Object
	{
	private:
		float range;
		float lastFireTime;
		float fireRate;
		Projectil projectil;
		std::unique_ptr<Animation> animation;
		TowerState currentState;
		float attackAnimationDuration;
		float attackAnimationTimer;

	public:
		Tower(float range = 0.0f,
			  Projectil projectil = Projectil(),
			  SDL_Color color = {0, 0, 255, 255})
			: range(range), lastFireTime(0), fireRate(1.0f),
			  projectil(std::move(projectil)), currentState(TowerState::Idle),
			  attackAnimationDuration(0.5f), attackAnimationTimer(0.0f)
		{
			this->setType(ObjectType::Tower);
			this->setColor(color);
			this->setSize(1, 1);
		}

		Tower(const Tower &) = delete;
		Tower &operator=(const Tower &) = delete;
		Tower(Tower &&other) noexcept
			: Object(std::move(other)), range(other.range),
			  lastFireTime(other.lastFireTime), fireRate(other.fireRate),
			  projectil(std::move(other.projectil)), animation(std::move(other.animation)),
			  currentState(other.currentState), attackAnimationDuration(other.attackAnimationDuration),
			  attackAnimationTimer(other.attackAnimationTimer)
		{
		}
		Tower &operator=(Tower &&other) noexcept
		{
			if (this != &other)
			{
				Object::operator=(std::move(other));
				range = other.range;
				lastFireTime = other.lastFireTime;
				fireRate = other.fireRate;
				projectil = std::move(other.projectil);
				animation = std::move(other.animation);
				currentState = other.currentState;
				attackAnimationDuration = other.attackAnimationDuration;
				attackAnimationTimer = other.attackAnimationTimer;
			}
			return *this;
		}

		void setRange(float range)
		{
			this->range = range;
		}
		float getRange() const
		{
			return this->range;
		}

		void setFireRate(float rate)
		{
			this->fireRate = rate;
		}
		float getFireRate() const
		{
			return this->fireRate;
		}

		void setProjectil(Projectil projectil)
		{
			this->projectil = std::move(projectil);
		}
		const Projectil &getProjectil() const
		{
			return this->projectil;
		}

		void setAnimation(std::unique_ptr<Animation> newAnimation)
		{
			this->animation = std::move(newAnimation);
		}

		Animation *getAnimation() const
		{
			return this->animation.get();
		}

		void setState(TowerState state);
		TowerState getState() const { return currentState; }
		void setAttackAnimationDuration(float duration) { attackAnimationDuration = duration; }

		std::unique_ptr<Projectil> findTargetAndTryFireAt(const std::vector<std::unique_ptr<Enemy>> &enemies, float currentTime);
		bool isWithinRange(const Vector &targetPosition) const;
		std::unique_ptr<Projectil> tryFireAt(Enemy *enemy, float currentTime);

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
	};
}

#endif