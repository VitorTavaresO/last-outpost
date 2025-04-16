#ifndef LAST_OUTPOST_TOWER_H
#define LAST_OUTPOST_TOWER_H

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/types.h>
#include <last-outpost/projectil.h>
#include <memory>

namespace Game
{
	class Enemy;
	class Tower : public Object
	{
	private:
		float range;
		float damage;
		float lastFireTime;
		float fireRate;

	public:
		Tower(float range = 0.0f, float damage = 0.0f, SDL_Color color = {0, 0, 255, 255})
			: range(range), damage(damage), lastFireTime(0), fireRate(1.0f)
		{
			this->setType(ObjectType::Tower);
			this->setColor(color);
			this->setSize(1, 1);
		}

		void setRange(float range)
		{
			this->range = range;
		}
		float getRange() const
		{
			return this->range;
		}

		void setDamage(float damage)
		{
			this->damage = damage;
		}
		float getDamage() const
		{
			return this->damage;
		}

		void setFireRate(float rate)
		{
			this->fireRate = rate;
		}
		float getFireRate() const
		{
			return this->fireRate;
		}

		void update(float deltaTime) override;
		void render(Graphics &graphics, float deltaTime) const override;
		bool isWithinRange(const Vector &targetPosition) const;

		std::unique_ptr<Projectil> tryFireAt(Enemy *enemy, float currentTime);
	};
}

#endif